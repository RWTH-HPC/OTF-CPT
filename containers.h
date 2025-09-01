#ifndef CONTAINERS_H
#define CONTAINERS_H 1

#include "debug.h"
#include "typedefs.h"

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

template <typename T> class Vector {
public:
  Vector() : begin_(), end_(), last_() {}

  ~Vector() {
    if (begin_)
      free(begin_);
  }

  // Default copy constructor would lead to double-free
  Vector(const Vector &) = delete;
  Vector &operator=(const Vector &) = delete;

  void Reset() {
    if (begin_)
      free(begin_);
    begin_ = 0;
    end_ = 0;
    last_ = 0;
  }

  uptr Size() const { return end_ - begin_; }

  bool Empty() const { return end_ == begin_; }

  T &operator[](uptr i) {
    DCHECK_LT(i, end_ - begin_);
    return begin_[i];
  }

  const T &operator[](uptr i) const {
    DCHECK_LT(i, end_ - begin_);
    return begin_[i];
  }

  T *PushBack() {
    EnsureSize(Size() + 1);
    T *p = &end_[-1];
    memset(p, 0, sizeof(*p));
    return p;
  }

  T *PushBack(const T &v) {
    EnsureSize(Size() + 1);
    T *p = &end_[-1];
    memcpy(p, &v, sizeof(*p));
    return p;
  }

  void PopBack() {
    DCHECK_GT(end_, begin_);
    end_--;
  }

  T &Back() {
    DCHECK_GT(end_, begin_);
    return end_[-1];
  }

  const T &getBack() const {
    DCHECK_GT(end_, begin_);
    return end_[-1];
  }

  void Resize(uptr size) {
    if (size == 0) {
      end_ = begin_;
      return;
    }
    uptr old_size = Size();
    if (size <= old_size) {
      end_ = begin_ + size;
      return;
    }
    EnsureSize(size);
    if (old_size < size) {
      memset(&begin_[old_size], 0,
             sizeof(begin_[old_size]) * (size - old_size));
    }
  }

  void Swap(Vector<T> &other) {
    T *tmp;
    tmp = other.begin_;
    other.begin_ = begin_;
    begin_ = tmp;
    tmp = other.end_;
    other.end_ = end_;
    end_ = tmp;
    tmp = other.last_;
    other.last_ = last_;
    last_ = tmp;
  }

  T *begin() { return begin_; }
  T *end() { return end_; }

  void *operator new(size_t size) { return malloc(size); }

  void operator delete(void *p) { free(p); }

protected:
  T *begin_;
  T *end_;
  T *last_;

private:
  void EnsureSize(uptr size) {
    if (size <= Size())
      return;
    if (size <= (uptr)(last_ - begin_)) {
      end_ = begin_ + size;
      return;
    }
    uptr cap0 = last_ - begin_;
    uptr cap = cap0 * 5 / 4; // 25% growth
    if (cap == 0)
      cap = 16;
    if (cap < size)
      cap = size;
    T *p = (T *)malloc(cap * sizeof(T));
    if (cap0) {
      memcpy(p, begin_, cap0 * sizeof(T));
      free(begin_);
    }
    begin_ = p;
    end_ = begin_ + size;
    last_ = begin_ + cap;
  }
};

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
};

template <typename K, typename V> class CompactHashMap {
public:
  using I = PairListItem<K, V>;
  using iterator = I *;
  using const_iterator = const I *;
  CompactHashMap() {}

  ~CompactHashMap() {
    if (begin_)
      free(begin_);
  }

  // Default copy constructor would lead to double-free
  CompactHashMap(const CompactHashMap &) = delete;
  CompactHashMap &operator=(const CompactHashMap &) = delete;

  void Reset() {
    if (begin_)
      free(begin_);
    begin_ = 0;
    end_ = 0;
    last_ = 0;
    mask_ = 0;
  }

  uptr Size() const { return end_ - begin_; }

  bool Empty() const { return end_ == begin_; }

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
    if (ReHash(Size() + 1)) {
      mask_ = (last_ - begin_) * 2 - 1;
    }
    it = end_ - 1;
    it->first = key;
    new (&it->second) V();
    it->Next = hbegin()[h & mask_];
    hbegin()[h & mask_] = it;
    return it->second;
  }

  iterator Find(const K &key) {
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

  Pair<iterator, bool> Insert(const Pair<K, V> &value) {
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
    if (ReHash(Size() + 1)) {
      mask_ = (last_ - begin_) * 2 - 1;
    }
    it = end_ - 1;
    memcpy(reinterpret_cast<Pair<K, V> *>(it), &value, sizeof(value));
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
  bool ReHash(uptr size) {
    if (size <= Size())
      return false;
    if (size <= (uptr)(last_ - begin_)) {
      end_ = begin_ + size;
      return false;
    }
    uptr cap0 = last_ - begin_;
    uptr cap = cap0 * 2; // 100% growth
    if (cap == 0)
      cap = 16;
    while (cap < size)
      cap *= 2;
    I *p = (I *)malloc(cap * sizeof(I) + cap * 2 * sizeof(I *));
    I **hp = (I **)(p + cap); //(I **)calloc(cap * 2, sizeof(I *));
    memset(hp, 0, cap * 2 * sizeof(I *));
    mask_ = cap * 2 - 1;
    if (cap0) {
      memcpy(p, begin_, cap0 * sizeof(I));
      for (auto pp = p; pp < p + cap0; pp++) {
        auto h = Hash<K>{}(pp->first);
        pp->Next = hp[h & mask_];
        hp[h & mask_] = pp;
      }
      free(begin_);
    }
    begin_ = p;
    end_ = begin_ + size;
    last_ = begin_ + cap;
    return true;
  }
};

struct CptStreamBuffer {
  char *buf;
  size_t cap;
  size_t pos = 0;

  // this struct always needs a buffer
  CptStreamBuffer() = delete;

  CptStreamBuffer(char *_buf, size_t _capacity) : buf(_buf), cap(_capacity) {}

  CptStreamBuffer &operator<<(const char *s) {
    if (!s || pos > cap - 1 || !buf)
      return *this;
    pos += snprintf(buf + pos, cap - pos, "%s", s);
    pos = (pos < cap - 1) ? pos : cap - 1;
    return *this;
  }

  CptStreamBuffer &operator<<(unsigned long long n) {
    if (pos > cap - 1 || !buf)
      return *this;
    pos += snprintf(buf + pos, cap - pos, "%llu", (unsigned long long)n);
    pos = (pos < cap - 1) ? pos : cap - 1;
    return *this;
  }

  CptStreamBuffer &operator<<(int n) {
    if (pos > cap - 1 || !buf)
      return *this;
    pos += snprintf(buf + pos, cap - pos, "%d", (int)n);
    pos = (pos < cap - 1) ? pos : cap - 1;
    return *this;
  }

  CptStreamBuffer &fflush(FILE *out) {
    if (pos > cap - 1 || !buf)
      return *this;
    fwrite(buf, sizeof(char), pos, out);
    return *this;
  }

  CptStreamBuffer &reset() {
    if (!buf)
      return *this;

    memset(buf, 0, cap);
    pos = 0;
    return *this;
  }
};

} // namespace __otfcpt

#endif
