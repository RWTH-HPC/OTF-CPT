/* StringStream.h -------------------------------------------------- */
#pragma once
#include "IOStream.h"
#include "String.h"
#include "StringBuf.h"

namespace __otfcpt {
class StringStream : public IOStream {
public:
  StringStream() : IOStream(&buf_), buf_(storage_) {}

  OStream &ostream() { return *this; }
  IStream &istream() { return *this; }

  const String &str() const { return storage_; }
  String &str() { return storage_; }

  void clear() { storage_.clear(); }

private:
  String storage_; // holds the characters
  StringBuf buf_;  // memory‑backed buffer
};
} // namespace __otfcpt