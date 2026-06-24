#pragma once

namespace __otfcpt {
template <typename T> struct Hash {
  size_t operator()(T __val) const noexcept {
    auto tmp = static_cast<size_t>(__val);
    return tmp + (tmp >> 5);
  }
};
template <typename T> struct Hash<T *> {
  size_t operator()(T *__val) const noexcept {
    auto tmp = reinterpret_cast<size_t>(__val);
    return tmp + (tmp >> 5);
  }
};
} // namespace __otfcpt