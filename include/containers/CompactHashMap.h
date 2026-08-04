#pragma once

#include "debug.h"
#include "memMove.h"
#include "typedefs.h"
#include <initializer_list>
#include <new> // Required for placement new
#include <type_traits>

#include "Hash.h"

namespace __cpt {

template <typename K, typename V> class Pair {
public:
  K first;
  V second;
  void *operator new(size_t size) { return malloc(size); }
  void operator delete(void *p) { free(p); }
};

template <typename K, typename V> class PairListItem : public Pair<K, V> {
public:
  PairListItem *Next;
  PairListItem &operator=(const Pair<K, V> &other) {
    this->first = other.first;
    this->second = other.second;
    Next = 0;
    return *this;
  }
};

template <typename K, typename V> class CompactHashMap : memMove {
public:
  using I = PairListItem<K, V>;
  using value_type = V;
  using iterator = I *;
  using const_iterator = const I *;
  CompactHashMap() {}

  ~CompactHashMap() { reset(); }

  // Default copy constructor would lead to double-free
  CompactHashMap(const CompactHashMap &) = delete;
  CompactHashMap &operator=(const CompactHashMap &) = delete;

  void reset() {
    if (begin_) {
      if constexpr (!std::is_trivially_destructible_v<I>) {
        for (I *p = begin_; p != end_; ++p) {
          p->~I();
        }
      }
      free(begin_);
    }
    begin_ = 0;
    end_ = 0;
    last_ = 0;
    mask_ = 0;
  }

  uptr size() const { return end_ - begin_; }

  bool empty() const { return end_ == begin_; }

  V &operator[](K key) {
    auto h = Hash<K>{}(key);
    iterator it{};
    if (mask_)
      it = hbegin()[h & mask_];
    if (it) {
      do {
        if (it->first == key)
          return it->second;
      } while ((it = it->Next));
    }
    if (ReHash(size() + 1)) {
      mask_ = (last_ - begin_) * 2 - 1;
    }
    it = end_ - 1;

    // Construct the item in raw memory using global placement new
    ::new (it) I();
    it->first = std::move(key);

    it->Next = hbegin()[h & mask_];
    hbegin()[h & mask_] = it;
    return it->second;
  }

  iterator find(const K &key) {
    auto h = Hash<K>{}(key);
    if (!mask_)
      return end_;
    iterator it = hbegin()[h & mask_];
    if (it) {
      do {
        if (it->first == key)
          return it;
      } while ((it = it->Next));
    }
    return end_;
  }

  Pair<iterator, bool> insert(const Pair<K, V> &value) {
    auto h = Hash<K>{}(value.first);
    iterator it{};
    if (mask_)
      it = hbegin()[h & mask_];
    if (it) {
      do {
        if (it->first == value.first)
          return Pair<iterator, bool>({it, false});
      } while ((it = it->Next));
    }
    if (ReHash(size() + 1)) {
      mask_ = (last_ - begin_) * 2 - 1;
    }
    it = end_ - 1;

    // Safely construct the empty container structures before assigning values
    ::new (it) I();
    *it = value;

    it->Next = hbegin()[h & mask_];
    hbegin()[h & mask_] = it;
    return Pair<iterator, bool>({it, true});
  }

  I *begin() { return begin_; }
  I *end() { return end_; }

  void *operator new(size_t size) { return malloc(size); }

  void operator delete(void *p) { free(p); }

protected:
  I *begin_{};
  I *end_{};
  I *last_{};
  uptr mask_{};
  I **hbegin() { return (I **)(last_); }

private:
  bool ReHash(uptr Size) {
    if (Size <= size())
      return false;
    if (Size <= (uptr)(last_ - begin_)) {
      end_ = begin_ + Size;
      return false;
    }
    uptr cap0 = last_ - begin_;
    uptr size0 = end_ - begin_;
    uptr cap = cap0 * 2; // 100% growth
    if (cap == 0)
      cap = 16;
    while (cap < Size)
      cap *= 2;
    I *p = (I *)malloc(cap * sizeof(I) + cap * 2 * sizeof(I *));
    I **hp = (I **)(p + cap);
    memset(hp, 0, cap * 2 * sizeof(I *));
    mask_ = cap * 2 - 1;
    if (cap0) {
      for (auto pp = p, bb = (I *)begin_; pp < p + size0; pp++, bb++) {
        // Move-construct old objects into uninitialized storage safely
        ::new (pp) I(std::move(*bb));
        // Run destructor on old object location to clean up resource
        // descriptors
        bb->~I();

        auto h = Hash<K>{}(pp->first);
        pp->Next = hp[h & mask_];
        hp[h & mask_] = pp;
      }
      free(begin_);
    }
    begin_ = p;
    end_ = begin_ + Size;
    last_ = begin_ + cap;
    return true;
  }
};
} // namespace __cpt