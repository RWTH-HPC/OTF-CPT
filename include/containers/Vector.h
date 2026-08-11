#pragma once

#include "debug.h"
#include "memMove.h"
#include "typedefs.h"
#include <initializer_list>
#include <iterator>
#include <new>
#include <type_traits>
#include <utility>

/*#define PRINT_CTOR 1
#define PRINT_DTOR 1
#define PRINT_ASSIGN 1*/

namespace __cpt {
template <typename T> class Vector : memMove {
public:
  using iterator = T *;
  using const_iterator = const T *;
  using value_type = T;

  Vector() : begin_(nullptr), end_(nullptr), last_(nullptr) {
#ifdef PRINT_CTOR
    printf("%s\n", __PRETTY_FUNCTION__);
#endif
  }

  ~Vector() {
#ifdef PRINT_DTOR
    printf("%s\n", __PRETTY_FUNCTION__);
#endif
    reset(); // Destroys elements and frees memory
  }

  Vector(const Vector &other) : Vector() {
#ifdef PRINT_CTOR
    printf("%s\n", __PRETTY_FUNCTION__);
#endif
    assign(other.begin(), other.end());
  }

  Vector(const_iterator first, const_iterator last) : Vector() {
#ifdef PRINT_CTOR
    printf("%s\n", __PRETTY_FUNCTION__);
#endif
    assign(first, last);
  }

  Vector(Vector &&other) noexcept {
#ifdef PRINT_CTOR
    printf("%s\n", __PRETTY_FUNCTION__);
#endif
    begin_ = other.begin_;
    end_ = other.end_;
    last_ = other.last_;
    other.begin_ = nullptr;
    other.end_ = nullptr;
    other.last_ = nullptr;
  }

  Vector(std::initializer_list<T> init) noexcept : Vector() {
#ifdef PRINT_CTOR
    printf("%s\n", __PRETTY_FUNCTION__);
#endif
    reserve(init.size());
    for (auto &elem : init) {
      ::new (end_) T(elem); // Placement new
      end_++;
    }
    CHECK_EQ(size(), init.size());
  }

  Vector &operator=(const Vector &other) {
#ifdef PRINT_ASSIGN
    printf("%s\n", __PRETTY_FUNCTION__);
#endif
    if (this == &other)
      return *this; // Guard against self-assignment
    assign(other.begin(), other.end());
    return *this;
  }

  Vector &operator=(Vector &&other) noexcept {
#ifdef PRINT_ASSIGN
    printf("%s\n", __PRETTY_FUNCTION__);
#endif
    if (this == &other)
      return *this;
    reset();
    begin_ = other.begin_;
    end_ = other.end_;
    last_ = other.last_;
    other.begin_ = nullptr;
    other.end_ = nullptr;
    other.last_ = nullptr;
    return *this;
  }

  // Destroys all elements and frees the underlying buffer
  void reset() {
    clear(); // Call destructors
    if (begin_)
      free(begin_);
    begin_ = nullptr;
    end_ = nullptr;
    last_ = nullptr;
  }

  // Destroys all elements, leaves capacity unchanged
  void clear() noexcept {
    if (!begin_ || !end_ || begin_ >= end_) {
      end_ = begin_;
      return;
    }

    // Only call destructors if the type actually needs destruction
    if constexpr (!std::is_trivially_destructible_v<T>) {
      for (T *p = begin_; p != end_; ++p) {
        p->~T();
      }
    }

    end_ = begin_;
  }

  uptr size() const { return end_ - begin_; }
  uptr capacity() const { return last_ - begin_; }
  bool empty() const { return end_ == begin_; }

  T &operator[](uptr i) {
    DCHECK_LT(i, end_ - begin_);
    return begin_[i];
  }

  const T &operator[](uptr i) const {
    DCHECK_LT(i, end_ - begin_);
    return begin_[i];
  }

  void push_back(const T &v) {
    if (end_ == last_)
      ensure_size(size() + 1);
    ::new (end_) T(v); // Construct object in raw memory
    end_++;
  }

  void push_back(T &&v) {
    if (end_ == last_)
      ensure_size(size() + 1);
    ::new (end_) T(std::move(v)); // Move construct object in raw memory
    end_++;
  }

  void pop_back() {
    DCHECK_GT(end_, begin_);
    end_--;
    end_->~T();
  }

  iterator insert(const_iterator pos, const T &item) {
    DCHECK_GE(pos, begin_);
    DCHECK_LE(pos, end_);

    uptr posIndex = pos - begin_;

    // Fast path: inserting at the end is just a push_back
    if (posIndex == size()) {
      push_back(item);
      return begin_ + posIndex;
    }

    // Ensure capacity
    if (size() == capacity()) {
      ensure_size(size() + 1);
    }

    T *insert_pos = begin_ + posIndex;

    // 1. Move the very last element into the uninitialized space at end_
    ::new (end_) T(std::move(*(end_ - 1)));

    // 2. Shift the rest of the elements one spot to the right
    // (using assignment because this memory is already initialized)
    for (T *p = end_ - 2; p >= insert_pos; --p) {
      *(p + 1) = std::move(*p);
    }

    // 3. Assign the new item into the opened slot
    *insert_pos = item;
    end_++;

    return begin_ + posIndex;
  }

  // Move-optimized overload for rvalues (e.g., Vector.insert(pos,
  // String("temp")))
  iterator insert(const_iterator pos, T &&item) {
    DCHECK_GE(pos, begin_);
    DCHECK_LE(pos, end_);

    uptr posIndex = pos - begin_;

    if (posIndex == size()) {
      push_back(std::move(item));
      return begin_ + posIndex;
    }

    if (size() == capacity()) {
      ensure_size(size() + 1);
    }

    T *insert_pos = begin_ + posIndex;

    ::new (end_) T(std::move(*(end_ - 1)));

    for (T *p = end_ - 2; p >= insert_pos; --p) {
      *(p + 1) = std::move(*p);
    }

    *insert_pos = std::move(item); // Move the new item in
    end_++;

    return begin_ + posIndex;
  }

  template <class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last) {
    DCHECK_GE(pos, begin_);
    DCHECK_LE(pos, end_);
    if (first == last)
      return const_cast<T *>(pos);

    uptr posIndex = pos - begin_;
    uptr itemsToInsert = std::distance(first, last);

    // Ensure we have enough memory
    if (size() + itemsToInsert > capacity()) {
      ensure_size(size() + itemsToInsert);
    }

    T *insert_pos = begin_ + posIndex;

    // Shift existing elements to the right (Backwards to avoid overwriting)
    for (T *p = end_ - 1; p >= insert_pos; --p) {
      if (p + itemsToInsert >= end_) {
        ::new (p + itemsToInsert)
            T(std::move(*p)); // Constructing into raw space
      } else {
        *(p + itemsToInsert) = std::move(*p); // Assigning into existing objects
      }
    }

    // Destroy the old shifted elements that are now conceptually "empty"
    uptr shiftOverlap = std::min((uptr)(end_ - insert_pos), itemsToInsert);
    if constexpr (!std::is_trivially_destructible_v<T>) {
      for (uptr i = 0; i < shiftOverlap; ++i) {
        (insert_pos + i)->~T();
      }
    }

    // Insert new elements
    for (uptr i = 0; i < itemsToInsert; ++i, ++first) {
      ::new (insert_pos + i) T(*first); // Placement new
    }

    end_ += itemsToInsert;
    return begin_ + posIndex;
  }

  iterator erase(iterator first, iterator last) {
    if (first == last)
      return last;
    DCHECK_GE(first, begin_);
    DCHECK_LE(last, end_);
    DCHECK_GE(last, first);

    uptr items_to_remove = last - first;

    // Move remaining elements to the left
    for (T *p = last; p != end_; ++p) {
      *(p - items_to_remove) = std::move(*p);
    }

    // Destroy the leftover elements at the tail
    if constexpr (!std::is_trivially_destructible_v<T>) {
      for (T *p = end_ - items_to_remove; p != end_; ++p) {
        p->~T();
      }
    }

    end_ -= items_to_remove;
    return first;
  }

  template <class InputIt> void assign(InputIt first, InputIt last) {
    clear(); // Destroy current elements
    size_t n = static_cast<size_t>(std::distance(first, last));
    reserve(n); // Allocate raw memory

    for (size_t i = 0; i < n; ++i, ++first) {
      ::new (begin_ + i) T(*first); // Copy construct each
    }
    end_ = begin_ + n;
  }

  T &back() {
    DCHECK_GT(end_, begin_);
    return end_[-1];
  }

  const T &getBack() const {
    DCHECK_GT(end_, begin_);
    return end_[-1];
  }

  T *data() { return begin_; }
  const T *data() const { return begin_; }

  void reserve(uptr new_cap) {
    if (new_cap > capacity())
      ensure_size(new_cap);
  }

  void resize(uptr new_size) {
    if (new_size < size()) {
      // Shrink: Destroy excess elements
      if constexpr (!std::is_trivially_destructible_v<T>) {
        for (T *p = begin_ + new_size; p != end_; ++p) {
          p->~T();
        }
      }
      end_ = begin_ + new_size;
    } else if (new_size > size()) {
      // Expand: Allocate memory and default construct elements
      reserve(new_size);
      for (T *p = end_; p != begin_ + new_size; ++p) {
        ::new (p) T(); // Default placement new
      }
      end_ = begin_ + new_size;
    }
  }

  void swap(Vector<T> &other) {
    std::swap(begin_, other.begin_);
    std::swap(end_, other.end_);
    std::swap(last_, other.last_);
  }

  constexpr iterator begin() { return begin_; }
  constexpr const_iterator begin() const { return begin_; }
  constexpr iterator end() { return end_; }
  constexpr const_iterator end() const { return end_; }

  constexpr auto rbegin() { return std::reverse_iterator(end_); }
  constexpr auto rbegin() const { return std::reverse_iterator(end_); }
  constexpr auto rend() { return std::reverse_iterator(begin_); }
  constexpr auto rend() const { return std::reverse_iterator(begin_); }

  void *operator new(size_t size) { return malloc(size); }
  void operator delete(void *p) { free(p); }

  bool operator==(const Vector &other) const noexcept {
    if (&other == this)
      return true;
    if (other.size() != size())
      return false;
    for (uptr i = 0; i < size(); ++i) {
      if (begin_[i] != other.begin_[i])
        return false;
    }
    return true;
  }

  bool operator!=(const Vector &other) const noexcept {
    return !(*this == other);
  }

protected:
  T *begin_;
  T *end_;
  T *last_;

private:
  void ensure_size(uptr required_cap) {
    if (required_cap <= capacity())
      return;

    uptr cap0 = capacity();
    uptr size0 = size();
    uptr cap = cap0 * 5 / 4; // 25% growth

    if (cap < 16)
      cap = 16;
    if (cap < required_cap)
      cap = required_cap;

    T *new_data = (T *)malloc(cap * sizeof(T));

    // Move elements to the new buffer using move construction
    for (uptr i = 0; i < size0; ++i) {
      ::new (new_data + i) T(std::move(begin_[i]));
      begin_[i].~T(); // Destroy the old element
    }

    if (begin_)
      free(begin_);

    begin_ = new_data;
    end_ = begin_ + size0;
    last_ = begin_ + cap;
  }
};
} // namespace __cpt