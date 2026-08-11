/*=====================================================================
   FileBuf.h   –   file‑backed concrete buffer (C‑only runtime)
   =====================================================================*/
#pragma once
#include "StreamBuf.h"
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <ios> // std::ios_base::openmode, SEEK_*

namespace __cpt {
class FileBuf : public StreamBuf {
public:
  /*-----------------------  ctors / dtor  ---------------------------*/
  FileBuf() = default;                   // start without a file
  explicit FileBuf(FILE *f) : file_(f) { // used when you already have a FILE*
    initialise_buffers();
  }
  ~FileBuf() override { close(); } // close automatically

  /*-----------------------  file management  ----------------------*/
  /** Open *pathname* using the given C++‑style mode bits.
      Returns this on success, nullptr on failure (exactly like
      std::basic_filebuf::open). */
  FileBuf *open(const char *pathname,
                std::ios_base::openmode mode = std::ios_base::in) {
    if (is_open())
      return nullptr; // already attached

    const char *fm = mode_to_c_mode(mode);
    if (!fm)
      return nullptr; // unsupported combination

    FILE *f = std::fopen(pathname, fm);
    if (!f)
      return nullptr; // errno is set by fopen

    file_ = f;
    initialise_buffers(); // (re)initialise read/write views
    return this;
  }

  /** Close the underlying FILE*. Returns this on success, nullptr on error
      (the same contract as std::basic_filebuf::close). */
  FileBuf *close() {
    if (!file_)
      return nullptr; // nothing to do

    // 1. Flush any pending output first
    if (sync() != 0) { // error while flushing
      std::fclose(file_);
      file_ = nullptr;
      invalidate_buffers();
      return nullptr;
    }

    // 2. Close the file
    int rc = std::fclose(file_); // 0 = success, EOF = failure
    file_ = nullptr;
    invalidate_buffers(); // make sure we do not use dangling pointers
    if (rc != 0)
      return nullptr;
    return this;
  }

  /** True iff a FILE* is attached and still open. */
  bool is_open() const noexcept { return file_ != nullptr; }

  /** Optional *bool* conversion – makes `if (buf)` work. */
  explicit operator bool() const noexcept { return is_open(); }

  /*-----------------------  positioning  --------------------------*/
protected:
  /* The four pure virtuals required by StreamBuf
   * --------------------------------*/

  /** Fill the read area.  Returns the first character (as an int) or EOF. */
  int_type underflow() override {
    if (!file_)
      return EOF; // no source
    std::size_t n = std::fread(readBuf_, 1, readSize_, file_);
    if (n == 0)
      return EOF; // true EOF

    // expose the freshly read chunk
    setg(readBuf_, readBuf_, readBuf_ + n);
    return traits_type::to_int_type(*gptr());
  }

  /** Write a character that could not be stored in the current put‑area. */
  int_type overflow(int_type ch) override {
    // 1. Flush whatever is already in the write buffer (if any)
    if (sync() != 0)
      std::abort(); // unrecoverable

    // 2. Write the character that triggered the overflow
    if (std::fputc(ch, file_) == EOF) {
      // In a production version you would propagate the error.
      // Here we simply abort, because we cannot throw exceptions.
      std::abort();
    }

    // 3. Reset the write view – a brand‑new empty put‑area.
    setp(nullptr, nullptr);
    return ch;
  }

  /** Flush pending output.  Returns 0 on success, non‑zero on error. */
  int sync() override {
    // Write whatever is still pending in the write buffer.
    std::size_t pending = static_cast<std::size_t>(writePtr_ - writeBase_);
    if (pending) {
      if (std::fwrite(writeBase_, 1, pending, file_) != pending)
        return EOF; // short write = error
    }
    // Also flush the underlying C stream.
    return std::fflush(file_);
  }

  /** Perform a low‑level seek on the underlying FILE*. */
  int do_seek(std::streamoff off, int whence) override {
    // 1. Flush any pending output – required by the C standard.
    if (sync() != 0)
      return -1;

    // 2. Seek the file.
    if (std::fseek(file_, off, whence) != 0)
      return -1;

    // 3. Invalidate the read buffer – the next read will call underflow().
    setg(nullptr, nullptr, nullptr);
    return 0;
  }

  /** Return the current file position (as reported by ftell). */
  std::streamoff do_tell() const override { return std::ftell(file_); }

  /*-------------------------------------------------------------
    xsputn – write a *contiguous* sequence of characters.
    -----------------------------------------------------------*/
  std::streamsize xsputn(const char_type *s, std::streamsize n) override {
    std::streamsize total_written = 0;

    while (n > 0) {
      // If we have no put‑area left, flush it and reset.
      if (writePtr_ == writeEnd_) {
        if (sync() != 0)
          break;                                 // write error → abort loop
        setp(writeBuf_, writeBuf_ + writeSize_); // fresh empty put‑area
      }

      std::size_t space = static_cast<std::size_t>(writeEnd_ - writePtr_);
      std::size_t to_copy = static_cast<std::size_t>(
          (static_cast<std::size_t>(n) < space) ? n : space);

      std::memcpy(writePtr_, s, to_copy);
      writePtr_ += to_copy;
      s += to_copy;
      n -= static_cast<std::streamsize>(to_copy);
      total_written += static_cast<std::streamsize>(to_copy);
    }

    return total_written; // number of characters written
  }

private:
  /*-----------------------  helper utilities  ----------------------*/

  /** Convert an std::ios_base::openmode bit‑mask to a C‑style fopen mode
      string.  Only the most common combinations are supported – the same
      subset that std::basic_filebuf supports. */
  static const char *mode_to_c_mode(std::ios_base::openmode mode) {
    // Simplify the mask (ignore bits that we do not interpret)
    bool in = (mode & std::ios_base::in) != 0;
    bool out = (mode & std::ios_base::out) != 0;
    bool app = (mode & std::ios_base::app) != 0;
    bool trunc = (mode & std::ios_base::trunc) != 0;
    // Note: std::ios_base::binary is ignored because on POSIX there is no
    // difference between binary and text mode; on Windows you could add
    // a 'b' suffix to the mode string if you need it.

    if (in && out) {
      // read‑write mode – decide between "r+", "w+" or "a+"
      if (app)
        return "a+";
      if (trunc)
        return "w+";
      return "r+";
    }
    if (in)
      return "r";
    if (out) {
      if (app)
        return "a";
      if (trunc)
        return "w";
      // std::ofstream opened with just ios::out uses "w"
      return "w";
    }
    return nullptr; // illegal combination (e.g. ios::app without out)
  }

  /** Allocate the static read / write buffers and make them visible to
      the base class via setg/setp.  Called after a successful open() or
      re‑open() of the underlying FILE*. */
  void initialise_buffers() {
    // Allocate (once) if they are not yet present.  Using malloc because
    // we are deliberately avoiding the C++ heap.
    if (!readBuf_)
      readBuf_ = static_cast<char *>(std::malloc(readSize_));
    if (!writeBuf_)
      writeBuf_ = static_cast<char *>(std::malloc(writeSize_));
    if (!readBuf_ || !writeBuf_)
      std::abort(); // out‑of‑memory

    // Start with empty areas – they will be filled by underflow()/overflow().
    setg(readBuf_, readBuf_, readBuf_);      // empty read view
    setp(writeBuf_, writeBuf_ + writeSize_); // fresh write view
    writeBase_ = writeBuf_; // remember the base of the put‑area
  }

  /** Release the internal buffers and clear the base‑class pointer fields. */
  void invalidate_buffers() noexcept {
    setg(nullptr, nullptr, nullptr);
    setp(nullptr, nullptr);
    writeBase_ = nullptr;
    // The actual storage is owned by the FileBuf object itself,
    // so we keep it allocated for possible reuse after a later open().
  }

  using traits_type = std::char_traits<char>;

  // -----------------------------------------------------------------
  //  Helper that mimics the public protected members of std::streambuf
  //  (setg / setp) but is *public* inside this derived class.
  // -----------------------------------------------------------------
  void setg(char *base, char *ptr, char *end) {
    readBuf_ = base;
    readPtr_ = ptr;
    readEnd_ = end;
  }

  /** Mirrors the protected interface of std::basic_streambuf::setp */
  void setp(char *base, char *end) noexcept {
    writeBase_ = base; // start of the put‑area
    writePtr_ = base;  // current put position
    writeEnd_ = end;   // one‑past‑last writable byte
  }

  /*-----------------------  data members  --------------------------*/

  FILE *file_ = nullptr; // underlying C file

  // -----------------------------------------------------------------
  //  Fixed‑size read/write buffers (4 KB each – change if you wish)
  // -----------------------------------------------------------------
  static constexpr std::size_t readSize_ = 4096;
  static constexpr std::size_t writeSize_ = 4096;

  char *readBuf_ = nullptr;  // raw storage for the input area
  char *writeBuf_ = nullptr; // raw storage for the output area

  /* writeBase_ is required by xsputn to compute the amount of data that
     is waiting to be flushed.  It always points to the beginning of the
     current put‑area (i.e. the same address that was passed as the first
     argument to setp). */
  char *writeBase_ = nullptr;
};
} // namespace __cpt
