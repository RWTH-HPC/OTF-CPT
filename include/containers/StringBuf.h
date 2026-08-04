#pragma once
#include "StreamBuf.h"
#include "String.h"

namespace __cpt {
class StringBuf : public StreamBuf {
public:
  explicit StringBuf(String &storage) : str_(storage) {}
  StringBuf(const StringBuf &) = delete;
  StringBuf(StringBuf &&) = default;

protected:
  int_type underflow() override {
    if (readPos_ >= str_.size())
      return EOF;
    // expose the remaining part of the string as the read area
    const char *start = str_.c_str() + readPos_;
    const char *end = str_.c_str() + str_.size();
    setg(const_cast<char *>(start), const_cast<char *>(start),
         const_cast<char *>(end));
    return traits_type::to_int_type(*gptr());
  }

  int_type overflow(int_type ch) override {
    str_.push_back(ch);
    // After pushing the character we can simply reset the write area
    // to an empty view – the next write will hit overflow again and
    // will call this function again.
    setp(nullptr, nullptr);
    return ch;
  }

  int sync() override { return 0; } // nothing to flush

  int do_seek(std::streamoff off, int whence) override {
    // Only support seeking from the beginning – enough for a stringbuf.
    if (whence != SEEK_SET)
      return -1;
    if (off < 0 || static_cast<std::size_t>(off) > str_.size())
      return -1;
    readPos_ = static_cast<std::size_t>(off);
    setg(nullptr, nullptr, nullptr); // invalidate read view
    return 0;
  }

  std::streamoff do_tell() const override {
    return static_cast<std::streamoff>(readPos_);
  }

  /*-------------------------------------------------------------
    xsputn – write a *contiguous* sequence of characters.
    -----------------------------------------------------------*/
  std::streamsize xsputn(const char_type *s, std::streamsize n) override {
    str_.append(s, n);
    return n; // number of characters written
  }

private:
  using traits_type = std::char_traits<char>;

  /* Set the *get* area.
     `base` – first byte of the area (may be nullptr for an empty view)
     `ptr`  – current read position (usually == base)
     `end`  – one‑past‑last readable byte */
  void setg(char *base, char *ptr, char *end) noexcept {
    readPtr_ = ptr;
    readEnd_ = end;
    // `base` is not stored separately because the fast‑path only needs
    // `gptr()` and `egptr()`.  Keeping it would only be useful for
    // implementing `pubseekpos` etc., which we do not need here.
    (void)base; // avoid “unused parameter” warning
  }

  /* Set the *put* area.
     `base` – first byte of the buffer (may be nullptr)
     `end`  – one‑past‑last writable byte   */
  void setp(char *base, char *end) noexcept {
    writePtr_ = base;
    writeEnd_ = end;
    // In a string‑buffer we do not need to keep `pbase` because every
    // character is appended directly to the underlying `MyString`.
    // The pointer values are only used by `overflow()` to decide whether
    // the buffer is “full”.  By always clearing the area (setting both to
    // nullptr) after each write we force `overflow()` to be called for
    // every subsequent character – which is exactly what we want for a
    // simple growing string.
  }
  /* -------------------------------------------------------------
     Private state that belongs to the string‑buffer implementation
     ------------------------------------------------------------- */
  String &str_;
  std::size_t readPos_ = 0; // index inside `str_` for the next read
};
} // namespace __cpt
