#pragma once
#include "StreamBuf.h"
#include "String.h"
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ios> // std::ios_base::iostate, openmode, etc.

namespace __cpt {
class IStream {
public:
  /*-------------------------------------------------------------
    ctor – the buffer is supplied by the user (no ownership)
    -------------------------------------------------------------*/
  explicit IStream(StreamBuf *buf) : buf_(buf) { clear(); }

  /*-------------------------------------------------------------
    ----  STATE‑HANDLING (Standard compliant definitions)  -----
    -------------------------------------------------------------*/
  using iostate = std::ios_base::iostate;

  iostate rdstate() const noexcept { return state_; }

  IStream &setstate(iostate mask) noexcept {
    state_ |= mask;
    return *this;
  }

  IStream &clear(iostate mask = std::ios_base::goodbit) noexcept {
    state_ = mask;
    return *this;
  }

  bool good() const noexcept { return (state_ == std::ios_base::goodbit); }
  bool fail() const noexcept {
    return (state_ & (std::ios_base::failbit | std::ios_base::badbit)) != 0;
  }
  bool bad() const noexcept { return (state_ & std::ios_base::badbit) != 0; }
  bool iseof() const noexcept { return (state_ & std::ios_base::eofbit) != 0; }
  bool eof() const noexcept { return (state_ & std::ios_base::eofbit) != 0; }

  /*-------------------------------------------------------------
    Extraction operators (int, unsigned int, String)
    -------------------------------------------------------------*/
  IStream &operator>>(int &v) {
    extract_int(v);
    return *this;
  }
  IStream &operator>>(unsigned int &v) {
    extract_uint(v);
    return *this;
  }
  IStream &operator>>(String &s) {
    extract_string(s);
    return *this;
  }

  /*-------------------------------------------------------------
    Fast‑path getline  (uses the public façade of StreamBuf)
    -------------------------------------------------------------*/
  bool getline(String &out, char delim = '\n') {
    // If the stream has already failed, abort immediately
    if (fail()) {
      return false;
    }

    out.clear();
    while (true) {
      const char *start = buf_->gptr();
      const char *end = buf_->egptr();

      if (start == end) { // buffer empty → refill
        // Use sbumpc() to safely fetch AND consume the character
        int c = buf_->sbumpc();
        if (c == EOF) {      // true EOF
          if (out.empty()) { // nothing extracted at all
            setstate(std::ios_base::failbit | std::ios_base::eofbit);
            return false;
          } else {
            setstate(std::ios_base::eofbit); // data was read before EOF
            return true;
          }
        }
        if (c == delim) { // delimiter read safely and consumed by sbumpc
          return true;
        }
        out.push_back(static_cast<char>(c));
        continue;
      }

      /* ---- search the current chunk for the delimiter -------- */
      const char *p = static_cast<const char *>(
          std::memchr(start, delim, static_cast<std::size_t>(end - start)));
      if (p) {
        std::size_t n = static_cast<std::size_t>(p - start);
        out.append(start, n);
        buf_->gbump(static_cast<int>(n + 1)); // skip the delimiter too
        return true;
      } else {
        std::size_t n = static_cast<std::size_t>(end - start);
        out.append(start, n);
        buf_->gbump(static_cast<int>(n));
      }
    }
  }

  IStream &seekoff(std::streamoff off, int whence) {
    if (fail()) {
      return *this;
    }
    if (buf_->seekoff(off, whence) != 0) {
      setstate(std::ios_base::failbit);
    } else {
      // Clear eofbit on successful seek, preserving other failure flags
      clear(state_ & ~std::ios_base::eofbit);
    }
    return *this;
  }

  std::streamoff tell() const { return buf_->tell(); }
  StreamBuf *rdbuf() { return buf_; }

  /*-------------------------------------------------------------
    Destructor (does *not* own the buffer)
    -------------------------------------------------------------*/
  ~IStream() = default;

  /** *bool* conversion – CRITICAL FIX: Makes `while (stream)` correctly check
   * for stream health */
  explicit operator bool() const noexcept { return !fail(); }

  /** Inverted stream state check operator `if (!stream)` */
  bool operator!() const noexcept { return fail(); }

private:
  /*------------------- helpers for extraction --------------------*/
  void skip_ws() {
    int c;
    // Advance pointer with gbump(1) when consuming whitespace characters
    while ((c = buf_->sgetc()) != EOF && std::isspace(c)) {
      buf_->gbump(1);
    }
  }

  void extract_int(int &v) {
    if (fail())
      return;
    skip_ws();
    v = 0;
    int sign = 1;
    int c = buf_->sgetc();
    if (c == '-') {
      sign = -1;
      buf_->gbump(1);
      c = buf_->sgetc();
    } else if (c == '+') {
      buf_->gbump(1);
      c = buf_->sgetc();
    }

    bool digits_read = false;
    while (c != EOF && std::isdigit(c)) {
      v = v * 10 + (c - '0');
      digits_read = true;
      buf_->gbump(1);
      c = buf_->sgetc();
    }
    v *= sign;

    if (c == EOF)
      setstate(std::ios_base::eofbit);
    if (!digits_read)
      setstate(std::ios_base::failbit);
  }

  void extract_uint(unsigned int &v) {
    if (fail())
      return;
    skip_ws();
    v = 0;
    int c = buf_->sgetc();
    bool digits_read = false;
    while (c != EOF && std::isdigit(c)) {
      v = v * 10 + (c - '0');
      digits_read = true;
      buf_->gbump(1);
      c = buf_->sgetc();
    }

    if (c == EOF)
      setstate(std::ios_base::eofbit);
    if (!digits_read)
      setstate(std::ios_base::failbit);
  }

  void extract_string(String &s) {
    if (fail())
      return;
    skip_ws();
    s.clear();
    int c = buf_->sgetc();
    bool chars_read = false;
    while (c != EOF && !std::isspace(c)) {
      s.push_back(static_cast<char>(c));
      chars_read = true;
      buf_->gbump(1);
      c = buf_->sgetc();
    }

    if (c == EOF)
      setstate(std::ios_base::eofbit);
    if (!chars_read)
      setstate(std::ios_base::failbit);
  }

  /*------------------- data members -----------------------------*/
  StreamBuf *buf_{}; // non‑owning pointer to the underlying buffer
  iostate state_ = std::ios_base::goodbit; // stream state (fail/eof/bad)
};
} // namespace __cpt