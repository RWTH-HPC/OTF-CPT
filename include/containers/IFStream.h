/*=====================================================================
   IFStream.h – a minimal, std‑compatible ifstream built on
                    FileBuf / IStream (C‑only runtime).
   =====================================================================*/

#pragma once
#include "FileBuf.h"
#include "IStream.h"
#include "String.h"
#include <cstdio> // FILE* (required for the ctor that takes a FILE*)
#include <ios>    // std::ios_base::openmode, std::ios_base::seekdir

namespace __otfcpt {
class IFStream : public IStream {
public:
  /*-------------------------------------------------------------
    Types that mimic the ones provided by std::basic_ifstream
    -------------------------------------------------------------*/
  using char_type = char;
  using int_type = int;
  using pos_type = std::streampos;
  using off_type = std::streamoff;
  using openmode = std::ios_base::openmode;
  using seekdir = std::ios_base::seekdir;

  /*-------------------------------------------------------------
    Constructors
    -------------------------------------------------------------*/
  /** Default‑construction – no file is open. */
  IFStream() : IStream(&buf_), buf_() {}

  /** Open a file immediately.  The default mode is `ios::in`. */
  explicit IFStream(const char *filename, openmode mode = std::ios_base::in)
      : IStream(&buf_), buf_() {
    open(filename, mode);
  }

  /** Destructor – closes the file automatically. */
  ~IFStream() { close(); }

  /*-------------------------------------------------------------
    File‑management functions (open / close / is_open)
    -------------------------------------------------------------*/
  /** Open a file.  Returns `true` on success, `false` on failure. */
  bool open(const char *filename, openmode mode = std::ios_base::in) {
    if (buf_.open(filename, mode) == nullptr) {
      // Opening failed → put the stream into fail state
      setstate(std::ios_base::failbit);
      return false;
    }
    // Opening succeeded → clear any previous error flags
    IStream::clear();
    return true;
  }

  /** Close the underlying file (if any). */
  void close() {
    if (buf_.is_open()) {
      auto rc = buf_.close(); // returns 0 on success
      if (rc == nullptr)      // error while closing
        setstate(std::ios_base::failbit);
      else
        IStream::clear(); // become a “good” stream again
    }
  }

  /** True iff a file is currently attached and open. */
  bool is_open() const noexcept { return buf_.is_open(); }

  /*-------------------------------------------------------------
    Positioning – seekg / tellg
    -------------------------------------------------------------*/
  /** Seek relative to the beginning / current / end. */
  IFStream &seekg(off_type off, seekdir dir = std::ios_base::beg) {
    // If the file is not open, or the stream is already in a
    // failed/bad state, executing a seek must safely do nothing.
    if (!is_open() || (IStream::rdstate() &
                       (std::ios_base::failbit | std::ios_base::badbit))) {
      return *this;
    }

    int whence;
    switch (dir) {
    case std::ios_base::beg:
      whence = SEEK_SET;
      break;
    case std::ios_base::cur:
      whence = SEEK_CUR;
      break;
    case std::ios_base::end:
      whence = SEEK_END;
      break;
    default:
      whence = SEEK_SET; // fallback, should never happen
    }

    if (!IStream::seekoff(off, whence)) {
      setstate(std::ios_base::failbit);
    } else {
      // Clear eofbit on a successful seek, matching standard std::ifstream
      // behavior
      IStream::clear(IStream::rdstate() & ~std::ios_base::eofbit);
    }

    return *this;
  }

  /** Seek to an absolute position (pos_type is just an alias for streampos). */
  IFStream &seekg(pos_type pos) {
    return seekg(static_cast<off_type>(pos), std::ios_base::beg);
  }

  /** Return the current read position (delegates to the buffer). */
  pos_type tellg() const {
    // Return an invalid position indicator if the file stream is
    // unusable
    if (!is_open() || (IStream::rdstate() &
                       (std::ios_base::failbit | std::ios_base::badbit))) {
      return static_cast<pos_type>(-1);
    }
    return static_cast<pos_type>(buf_.tell());
  }

  /*-------------------------------------------------------------
    Stream state handling – clear / rdstate / good / eof …
    -------------------------------------------------------------*/
  using iostate = std::ios_base::iostate;

  IFStream &setstate(iostate mask) noexcept {
    IStream::setstate(mask);
    return *this;
  }

  IFStream &clear(iostate mask = std::ios_base::goodbit) noexcept {
    IStream::clear(mask);
    return *this;
  }

  /*-------------------------------------------------------------
    Extraction operators (forward to the inner IStream)
    -------------------------------------------------------------*/

  /** Fast‑path getline that works on a String (identical to the one
      in IStream, just forwarded for convenience). */
  bool getline(String &out, char delim = '\n') {
    return IStream::getline(out, delim);
  }

  /*-------------------------------------------------------------
    Miscellaneous helpers (optional, but convenient)
    -------------------------------------------------------------*/
  /** Return the underlying buffer – useful if you need direct access. */
  FileBuf *rdbuf() noexcept { return &buf_; }
  const FileBuf *rdbuf() const noexcept { return &buf_; }

  void *operator new(size_t size) { return malloc(size); }

  void operator delete(void *p) { free(p); }

private:
  FileBuf buf_; // owns the C file + internal buffers
};
} // namespace __otfcpt
