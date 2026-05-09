#pragma once

#include <cstring>

namespace __otfcpt {
class memMove {
public:
  template <typename T> void moveItems(T *dest, T *src, size_t n) {
    if (!n)
      return;
    if (std::is_trivially_copyable_v<T>)
      memmove(dest, src, n * sizeof(T));
    else {
      if (dest < src)
        for (size_t i = 0; i < n; i++, dest++, src++)
          *dest = std::move(*src);
      else {
        auto dRIt = dest + n - 1;
        auto sRIt = src + n - 1;
        for (size_t i = 0; i < n; i++, dRIt--, sRIt--)
          *dRIt = std::move(*sRIt);
      }
    }
  }
  template <typename T> void copyItems(T *dest, const T *src, size_t n) {
    if (!n)
      return;
    if (std::is_trivially_copyable_v<T>)
      memmove(dest, src, n * sizeof(T));
    else {
      if (dest < src)
        for (size_t i = 0; i < n; i++, dest++, src++)
          *dest = *src;
      else {
        auto dRIt = dest + n - 1;
        auto sRIt = src + n - 1;
        for (size_t i = 0; i < n; i++, dRIt--, sRIt--)
          *dRIt = *sRIt;
      }
    }
  }
  void setItems(int *dest, size_t n, int v) {
    if (!n)
      return;
    memset(dest, v, n * sizeof(int));
  }
  template <typename T> void setItems(T *dest, size_t n, T v = T{}) {
    if (!n)
      return;
    for (size_t i = 0; i < n; i++, dest++)
      *dest = v;
  }
};
} // namespace __otfcpt
