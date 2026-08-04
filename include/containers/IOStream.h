#pragma once
#include "IStream.h"
#include "OStream.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace __cpt {
class IOStream : public OStream, public IStream {
public:
  explicit IOStream(StreamBuf *buf) : OStream(buf), IStream(buf) {}
};
} // namespace __cpt