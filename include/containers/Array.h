#pragma once

#include "debug.h"
#include "typedefs.h"
#include <initializer_list>
#include <type_traits>

namespace __otfcpt {

template <typename T, std::size_t N> class Array {
protected:
  T begin_[N]{}; // zero‑initialized (if T is POD)
public:
  constexpr Array() = default;
  // -------------------------------------------------
  // list constructor
  // -------------------------------------------------
  //  Not declared as `explicit`, so that
  //  copy‑list‑initialization (`Array a = {…}`) works.
  constexpr Array(std::initializer_list<T> init) noexcept {
    DCHECK_LE(init.size(), N);

    // Copy values from initializer list to the internal
    size_t i = 0;
    for (auto elem : init) {
      begin_[i++] = elem;
    }

    // Init remaining elements to default initializer
    for (i = init.size(); i < N; i++) {
      begin_[i] = T{};
    }
  }

  constexpr std::size_t size() const { return N; }
  constexpr T *data() { return begin_; }
  constexpr const T *data() const { return begin_; }
  constexpr T *begin() { return begin_; }
  constexpr const T *begin() const { return begin_; }
  constexpr T *end() { return begin_ + N; }
  constexpr const T *end() const { return begin_ + N; }
  constexpr const T &operator[](uptr i) const {
    DCHECK_LT(i, N);
    return begin_[i];
  }
  constexpr T &operator[](uptr i) {
    DCHECK_LT(i, N);
    return begin_[i];
  }
  /* -------------------------------------------------
        operator+=  --  elementwise addition
       ------------------------------------------------- */
  // only activ, if both elements have same base type
  template <typename U>
  constexpr std::enable_if_t<std::is_same<T, U>::value, Array &> operator+=(
      const Array<U, N> &rhs) noexcept(std::is_nothrow_assignable_v<T &, T>) {
    // std::transform might be elegant, but a simple loop avoids dependency to
    // <algorithm>.
    for (std::size_t i = 0; i < N; ++i) {
      begin_[i] += rhs.begin_[i];
    }
    return *this;
  }
  /* -------------------------------------------------
    operator+ --  using operator+=
   ------------------------------------------------- */
  // implemented as non-member function, so that
  // lhs and rhs can be implizitly converted.
  template <typename U>
  friend constexpr std::enable_if_t<std::is_same<T, U>::value, Array<T, N>>
  operator+(Array<T, N> lhs, const Array<U, N> &rhs) noexcept {
    lhs += rhs; // using above += operator
    return lhs; // Return‑By‑Value (RVO / NRVO)
  }
  /* -------------------------------------------------
          operator-=  --  elementwise substraction
         ------------------------------------------------- */
  // only activ, if both elements have same base type
  template <typename U>
  constexpr std::enable_if_t<std::is_same<T, U>::value, Array &> operator-=(
      const Array<U, N> &rhs) noexcept(std::is_nothrow_assignable_v<T &, T>) {
    // std::transform might be elegant, but a simple loop avoids dependency to
    // <algorithm>.
    for (std::size_t i = 0; i < N; ++i) {
      begin_[i] -= rhs.begin_[i];
    }
    return *this;
  }
  /* -------------------------------------------------
    operator- --  using operator-=
   ------------------------------------------------- */
  // implemented as non-member function, so that
  // lhs and rhs can be implizitly converted.
  template <typename U>
  friend constexpr std::enable_if_t<std::is_same<T, U>::value, Array<T, N>>
  operator-(Array<T, N> lhs, const Array<U, N> &rhs) noexcept {
    lhs -= rhs; // using above -= operator
    return lhs; // Return‑By‑Value (RVO / NRVO)
  }
};
} // namespace __otfcpt
