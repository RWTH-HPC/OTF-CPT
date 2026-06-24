#pragma once
#include "Manip.h" // <-- our custom manipulators
#include "StreamBuf.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ios>     // for std::ios_base::fmtflags (if we need more)
#include <ostream> // for std::endl

namespace __otfcpt {
class OStream {
public:
  explicit OStream(StreamBuf *buf) : buf_(buf), width_(0), left_align_(false) {}

  /* ---------- manipulators ---------- */
  //   std::setw analogue
  OStream &operator<<(setw_t w) {
    width_ = w.width; // store the width for the next output
    // When a width is supplied we keep the current alignment flag,
    // because the user may write:  out << left << setw(6) << 42;
    return *this;
  }
  OStream &operator<<(int v) {
    fmt("%d", v);
    return *this;
  }
  OStream &operator<<(unsigned int v) {
    fmt("%u", v);
    return *this;
  }
  OStream &operator<<(unsigned long v) {
    fmt("%lu", v);
    return *this;
  }
  OStream &operator<<(void const *p) {
    fmt("%p", p);
    return *this;
  }
  OStream &operator<<(const char *s) {
    // strings are not affected by width/alignment (the same rule as
    // std::ostream)
    while (*s)
      buf_->sputc(*s++);
    return *this;
  }

  OStream &operator<<(const String &s) {
    const char *p = s.c_str();
    std::size_t n = s.size();
    // Write the whole string in one go – `xsputn` (via MyBuf) will buffer it.
    buf_->sputn(p, static_cast<std::streamsize>(n));
    return *this;
  }
  //   std::left analogue
  OStream &operator<<(left_t) {
    left_align_ = true; // subsequent formatted output is left‑justified
    return *this;
  }
  OStream &operator<<(std::ostream &(*manip)(std::ostream &)) {
    if (manip == static_cast<std::ostream &(*)(std::ostream &)>(std::endl)) {
      buf_->sputc('\n');
      buf_->flush();
    }
    return *this;
  }
  //   std::right, std::left
  OStream &operator<<(std::ios_base &(*manip)(std::ios_base &)) {
    // Only handle the manipulators we know about.
    // For std::endl we recognise the exact function address.
    // std::left is a standard manipulator that set the justification to left.
    if (manip == static_cast<std::ios_base &(*)(std::ios_base &)>(std::left)) {
      left_align_ = true;
    }
    // std::right is a standard manipulator that resets the justification.
    else if (manip ==
             static_cast<std::ios_base &(*)(std::ios_base &)>(std::right)) {
      left_align_ = false;
    }
    // add more standard manipulators here if you need them (hex, dec, …)
    return *this;
  }

  OStream &set_width(int w) {
    width_ = w;
    return *this;
  }
  void flush() { buf_->flush(); }

  /* seeking */
  OStream &seekoff(std::streamoff off, int whence) {
    buf_->seekoff(off, whence);
    return *this;
  }
  std::streamoff tell() const { return buf_->tell(); }

public:
  /* -----------------------------------------------------------------
     State handling (mirrors std::ostream)
     ----------------------------------------------------------------- */
  using iostate = std::ios_base::iostate;
  iostate rdstate() const noexcept { return state_; }

  OStream &setstate(iostate mask) noexcept {
    state_ |= mask;
    return *this;
  }
  OStream &clear(iostate mask = std::ios_base::goodbit) noexcept {
    state_ = mask;
    return *this;
  }
  bool good() const noexcept { return state_ == std::ios_base::goodbit; }
  bool fail() const noexcept {
    return (state_ & (std::ios_base::failbit | std::ios_base::badbit));
  }
  bool bad() const noexcept { return (state_ & std::ios_base::badbit); }
  bool iseof() const noexcept { return (state_ & std::ios_base::eofbit); }
  bool eof() const noexcept { return (state_ & std::ios_base::eofbit); }

private:
  void fmt(const char *fmt_spec, ...) {
    char tmp[128]; // enough for most cases
    std::va_list ap;
    va_start(ap, fmt_spec);

    // Build a *dynamic* format string that incorporates width/justification.
    //   - right‑justified →  "%*d"
    //   - left‑justified  →  "%-*d"
    //   - no width        →  just the original specifier ("%d", "%u", …)
    if (width_ != 0) {
      // Build a temporary format like "%*d" or "%-*d"
      char dyn_fmt[32];
      std::snprintf(dyn_fmt, sizeof(dyn_fmt), "%%%s%d%s",
                    (left_align_ ? "-" : ""), width_, fmt_spec + 1);
      std::vsnprintf(tmp, sizeof(tmp), dyn_fmt, ap);
    } else {
      // No width – use the specifier unchanged
      std::vsnprintf(tmp, sizeof(tmp), fmt_spec, ap);
    }
    va_end(ap);
    // Write the resulting text to the underlying buffer (via xsputn).
    buf_->sputn(tmp, static_cast<std::streamsize>(strnlen(tmp, sizeof(tmp))));

    // Reset width after the *next* formatted output, exactly as std::ostream
    // does.
    width_ = 0;
    left_align_ = false;
  }

private:
  StreamBuf *buf_;  // non‑owning pointer to the underlying buffer
  int width_;       // width supplied by setw()
  bool left_align_; // true → left‑justified, false → right‑justified
  iostate state_ = std::ios_base::goodbit;
};
} // namespace __otfcpt