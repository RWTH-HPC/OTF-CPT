#pragma once

#include "debug.h"
#include "typedefs.h"
#include <cstring>
#include <initializer_list>
#include <streambuf>
#include <type_traits>

#include "Hash.h"
#include "Vector.h"

namespace __cpt {
class String {
public:
  // -----------------------------------------------------------------
  // Types & constants
  // -----------------------------------------------------------------
  using size_type = std::size_t;
  static constexpr size_type npos = static_cast<size_type>(-1);
  using iterator = char *;
  using const_iterator = const char *;

  // -----------------------------------------------------------------
  // Constructors / assignment
  // -----------------------------------------------------------------
  String() = default;                                   // empty string
  String(const char *s) { assign_cstr(s); }             // from C‑string
  String(const String &other) = default;                // copy
  String(String &&other) noexcept = default;            // move
  String &operator=(const String &other) = default;     // copy assign
  String &operator=(String &&other) noexcept = default; // move assign

  // -----------------------------------------------------------------
  // Basic observers
  // -----------------------------------------------------------------
  size_type length() const noexcept {
    return data_.empty() ? 0 : data_.size() - 1;
  }
  size_type size() const noexcept {
    return data_.empty() ? 0 : data_.size() - 1;
  }
  size_type capacity() const {
    return !data_.capacity() ? 0 : data_.capacity() - 1;
  }
  bool empty() const noexcept { return size() == 0; }
  void reserve(size_type size) {
    if (size > capacity())
      data_.reserve(size + 1);
    DCHECK_GE(capacity(), size);
  }
  Vector<char> &raw() { return data_; }
  const Vector<char> &raw() const { return data_; }

  const char *c_str() const noexcept {
    return data_.empty() ? "" : data_.data();
  }

  // -----------------------------------------------------------------
  // Element access (optional, but handy)
  // -----------------------------------------------------------------
  char &operator[](size_type i) { return data_[i]; }
  const char &operator[](size_type i) const { return data_[i]; }

  // -----------------------------------------------------------------
  // Modifiers required by the request
  // -----------------------------------------------------------------
  /** Append another String */
  String &append(const String &str) {
    data_.insert(end(), str.begin(), str.end());
    return *this;
  }
  String &append(char c) {
    data_.insert(end(), c);
    return *this;
  }
  String &append(const char *c, int n) {
    data_.insert(end(), c, c + n);
    return *this;
  }

  /** Insert another String at position pos */
  String &insert(size_type pos, const String &str) {
    DCHECK_LT(pos, size() - 1);
    // if (pos > size())
    //     throw std::out_of_range("String::insert: position out of range");
    data_.insert(data_.begin() + static_cast<std::ptrdiff_t>(pos), str.begin(),
                 str.end());
    return *this;
  }

  /** Return a substring [pos, pos+count) */
  String substr(size_type pos = 0, size_type count = npos) const {
    DCHECK_LT(pos, size());
    // if (pos > size())
    //     throw std::out_of_range("String::substr: position out of range");
    size_type realCount = std::min(count, size() - pos);
    String result;
    result.data_.assign(data_.begin() + static_cast<std::ptrdiff_t>(pos),
                        data_.begin() +
                            static_cast<std::ptrdiff_t>(pos + realCount));
    result.data_.push_back('\0');
    return result;
  }

  /** Find the first occurrence of needle starting at pos */
  size_type find(const String &needle, size_type pos = 0) const noexcept {
    if (needle.empty())
      return pos <= size() ? pos : npos;
    if (pos >= size())
      return npos;

    auto res = strstr(c_str(), needle.c_str());
    if (!res)
      return npos;
    return res - c_str();
  }

  template <class InputIt> String &assign(InputIt first, InputIt last) {
    data_.assign(first, last);
    if (data_.back() != '\0')
      data_.push_back('\0');
    return *this;
  }

  // -----------------------------------------------------------------
  // Comparison (helpful for testing / printing)
  // -----------------------------------------------------------------
  bool operator==(const String &other) const noexcept {
    return data_ == other.data_;
  }
  bool operator!=(const String &other) const noexcept {
    return !(*this == other);
  }

  bool operator==(const char *other) const noexcept {
    if (other == data_.data())
      return true;
    if (strlen(other) != size())
      return false;
    return memcmp(data_.data(), other, size()) == 0;
  }
  bool operator!=(const char *other) const noexcept {
    return !(*this == other);
  }

  constexpr iterator begin() { return data_.begin(); }
  constexpr const_iterator begin() const { return data_.begin(); }
  constexpr iterator end() { return data_.end() - 1; }
  constexpr const_iterator end() const { return data_.end() - 1; }

  friend std::ostream &operator<<(std::ostream &out, const String &c) {
    out << std::string(c.c_str());
    return out;
  }
  friend String operator+(const String &lhs, const String &rhs) {
    String ret{lhs};
    return ret.append(rhs);
  }
  friend String operator+(const String &lhs, char rhs) {
    String ret{lhs};
    return ret.append(rhs);
  }
  void resize(size_type count) { data_.resize(count); }
  void push_back(char c) {
    if (empty()) {
      data_.push_back(c);
      data_.push_back('\0');
    } else
      data_.insert(end(), c);
  } // add one character
  void clear() noexcept { data_.clear(); } // remove all characters
private:
  Vector<char> data_{}; // holds characters *with* a terminating '\0'

  void assign_cstr(const char *s) {
    if (s) {
      size_type len = static_cast<size_type>(std::strlen(s));
      data_.assign(s, s + len + 1);
    }
  }
};

template <> struct Hash<String> {
  size_t operator()(const String &__val) const noexcept {
    constexpr std::uint64_t FNV_OFFSET_BASIS_64 = 14695981039346656037ULL;
    constexpr std::uint64_t FNV_PRIME_64 = 1099511628211ULL;
    std::uint64_t h = FNV_OFFSET_BASIS_64;
    for (auto c : __val) {
      h ^= static_cast<unsigned char>(c);
      h *= FNV_PRIME_64;
    }
    return static_cast<std::size_t>(h);
  }
};

} // namespace __cpt