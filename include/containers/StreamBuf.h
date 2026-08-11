#pragma once
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ios> // Required for std::streamoff and std::streamsize

namespace __cpt {
class StreamBuf {
public:
  using char_type = char;
  using int_type = int; // same convention as std::basic_streambuf

  StreamBuf() = default;
  StreamBuf(const StreamBuf &) = delete;
  StreamBuf(StreamBuf &&) = default;
  virtual ~StreamBuf() = default;

  static void *operator new(std::size_t n) { return malloc(n); }
  static void operator delete(void *p) noexcept { free(p); }
  static void operator delete(void *p, std::size_t) noexcept { free(p); }

  /* -------------------------------------------------------------
     Public façade – exactly the members that the fast‑path
     getline (and any other code that wants direct buffer access)
     expects.
     ------------------------------------------------------------- */
  const char_type *gptr() const noexcept { return readPtr_; }
  const char_type *egptr() const noexcept { return readEnd_; }

  // CRITICAL FIX: Changed parameter type to 'int' to match standard conventions
  void gbump(int n) noexcept { readPtr_ += n; }

  /* -------------------------------------------------------------
     Primitive input operation used by the standard‑style
     extraction operators (operator>>). It peeks at the character
     without advancing the pointer.
     ------------------------------------------------------------- */
  int_type sgetc() {
    if (readPtr_ >= readEnd_) {
      if (underflow() == EOF)
        return EOF;
    }
    return static_cast<unsigned char>(*readPtr_);
  }

  /* -------------------------------------------------------------
     Fetches the current character AND advances the read pointer by 1.
     ------------------------------------------------------------- */
  int_type sbumpc() {
    if (readPtr_ >= readEnd_) {
      if (underflow() == EOF)
        return EOF;
    }
    int_type ch = static_cast<unsigned char>(*readPtr_);
    gbump(1);
    return ch;
  }

  /* -------------------------------------------------------------
     Primitive output operation used by the overloaded operator<<.
     ------------------------------------------------------------- */
  void sputc(char_type c) {
    if (writePtr_ == writeEnd_)
      overflow(c);
    else {
      *writePtr_ = c;
      ++writePtr_;
    }
  }

  /* -------------------------------------------------------------
     Flush the output buffer – public because a user may call
     `stream.flush()`.
     ------------------------------------------------------------- */
  void flush() { sync(); }

  /* -------------------------------------------------------------
     Seek / tell – default implementation forwards to the virtual
     functions that the concrete class has to provide.
     ------------------------------------------------------------- */
  int seekoff(std::streamoff off, int whence) {
    if (sync() != 0)
      return -1;                     // make sure buffers are consistent
    readPtr_ = readEnd_ = nullptr;   // invalidate the input view
    writePtr_ = writeEnd_ = nullptr; // invalidate the output view
    return do_seek(off, whence);
  }
  std::streamoff tell() const { return do_tell(); }
  std::streamsize sputn(const char_type *s, std::streamsize count) {
    return xsputn(s, count);
  }

  /* -------------------------------------------------------------
     Derived classes **must** implement the following virtuals.
     ------------------------------------------------------------- */
protected:
  /* Called when the input buffer is empty.  Must fill the read
     area and set `readPtr_`/`readEnd_`.  Return EOF on end‑of‑file. */
  virtual int_type underflow() = 0;

  /* Called when the output buffer is full (or when a character is
     explicitly pushed).  The default implementation writes the
     already‑filled part and then stores the new character. */
  virtual int_type overflow(int_type ch) = 0;

  /* Flush any buffered output.  Return 0 on success. */
  virtual int sync() = 0;

  /* Seek the underlying object.  Return 0 on success, -1 on error. */
  virtual int do_seek(std::streamoff off, int whence) = 0;
  virtual std::streamoff do_tell() const = 0;
  virtual std::streamsize xsputn(const char_type *s, std::streamsize count) = 0;

  /* -------------------------------------------------------------
     The buffer pointers – protected so derived classes may
     initialise them directly.
     ------------------------------------------------------------- */
  const char_type *readPtr_ = nullptr;
  const char_type *readEnd_ = nullptr;

  char_type *writePtr_ = nullptr;
  char_type *writeEnd_ = nullptr;
};
} // namespace __cpt