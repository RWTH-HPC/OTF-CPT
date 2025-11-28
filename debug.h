#ifndef DEBUG_H
#define DEBUG_H

#include "typedefs.h"
#include <mutex>
#ifdef USE_BACKWARD
#define CALLSTACK_SIZE_PRETTY 32
#define SKIP_FRAMES 3
#include <external/backward-cpp/backward.hpp>
#endif

// Fallback stack trace
#define CALLSTACK_SIZE 20

using namespace __otfcpt;

extern std::atomic<uint32_t> current_verbosity;
inline std::mutex checkOutputMutex;

inline void SetVerbosity(int verbosity) {
  current_verbosity.store(verbosity, std::memory_order_relaxed);
}
inline int Verbosity() {
  return current_verbosity.load(std::memory_order_relaxed);
}

void print_stack();
void pretty_print_stack();

void NORETURN Die();

void CheckFailed(const char *file, int line, const char *cond, u64 v1, u64 v2);

#define RAW_CHECK_MSG(expr, msg, ...)                                          \
  do {                                                                         \
    if (UNLIKELY(!(expr))) {                                                   \
      const char *msgs[] = {msg, __VA_ARGS__};                                 \
      for (const char *m : msgs)                                               \
        RawWrite(m);                                                           \
      Die();                                                                   \
    }                                                                          \
  } while (0)

#define RAW_CHECK(expr) RAW_CHECK_MSG(expr, #expr "\n", )
#define RAW_CHECK_VA(expr, ...) RAW_CHECK_MSG(expr, #expr "\n", __VA_ARGS__)

#define CHECK_IMPL(c1, op, c2)                                                 \
  do {                                                                         \
    u64 v1 = (u64)(c1);                                                        \
    u64 v2 = (u64)(c2);                                                        \
    if (UNLIKELY(!(v1 op v2))) {                                               \
      std::lock_guard<std::mutex> guard(checkOutputMutex);                     \
      CheckFailed(__FILE__, __LINE__, "(" #c1 ") " #op " (" #c2 ")", v1, v2);  \
    }                                                                          \
  } while (false) /**/

#define CHECK_IMPL_VA(c1, op, c2, ...)                                         \
  do {                                                                         \
    u64 v1 = (u64)(c1);                                                        \
    u64 v2 = (u64)(c2);                                                        \
    if (UNLIKELY(!(v1 op v2))) {                                               \
      std::lock_guard<std::mutex> guard(checkOutputMutex);                     \
      const char *msgs[] = {__VA_ARGS__};                                      \
      for (const char *m : msgs)                                               \
        fprintf((get_otfcpt_flags()->output ? get_otfcpt_flags()->output       \
                                            : stderr),                         \
                "%s", m);                                                      \
      CheckFailed(__FILE__, __LINE__, "(" #c1 ") " #op " (" #c2 ")", v1, v2);  \
    }                                                                          \
  } while (false) /**/

#define VPrintf(level, ...)                                                    \
  do {                                                                         \
    if ((uptr)Verbosity() >= (level))                                          \
      printf(__VA_ARGS__);                                                     \
  } while (0)

#define CHECK(a) CHECK_IMPL((a), !=, 0)
#define CHECK_NOT(a) CHECK_IMPL((a), ==, 0)
#define CHECK_EQ(a, b) CHECK_IMPL((a), ==, (b))
#define CHECK_NE(a, b) CHECK_IMPL((a), !=, (b))
#define CHECK_LT(a, b) CHECK_IMPL((a), <, (b))
#define CHECK_LE(a, b) CHECK_IMPL((a), <=, (b))
#define CHECK_GT(a, b) CHECK_IMPL((a), >, (b))
#define CHECK_GE(a, b) CHECK_IMPL((a), >=, (b))
#define CHECK_OR(a, b) CHECK_IMPL((a), ||, (b))
#define CHECK_AND(a, b) CHECK_IMPL((a), &&, (b))
#define CHECK_VA(a, ...) CHECK_IMPL_VA((a), !=, 0, __VA_ARGS__)
#define CHECK_EQ_VA(a, b, ...) CHECK_IMPL_VA((a), ==, (b), __VA_ARGS__)

#ifndef NDEBUG
#define DCHECK(a) CHECK(a)
#define DCHECK_NOT(a) CHECK_NOT(a)
#define DCHECK_EQ(a, b) CHECK_EQ(a, b)
#define DCHECK_NE(a, b) CHECK_NE(a, b)
#define DCHECK_LT(a, b) CHECK_LT(a, b)
#define DCHECK_LE(a, b) CHECK_LE(a, b)
#define DCHECK_GT(a, b) CHECK_GT(a, b)
#define DCHECK_GE(a, b) CHECK_GE(a, b)
#define DCHECK_OR(a, b) CHECK_OR(a, b)
#define DCHECK_AND(a, b) CHECK_AND(a, b)
#define DCHECK_VA(a, ...) CHECK_VA(a, __VA_ARGS__)
#define DCHECK_EQ_VA(a, b, ...) CHECK_EQ_VA(a, b, __VA_ARGS__)
#else
#define DCHECK(a)
#define DCHECK_EQ(a, b)
#define DCHECK_NE(a, b)
#define DCHECK_LT(a, b)
#define DCHECK_LE(a, b)
#define DCHECK_GT(a, b)
#define DCHECK_GE(a, b)
#define DCHECK_OR(a, b)
#define DCHECK_AND(a, b)
#define DCHECK_VA(a, ...)
#define DCHECK_EQ_VA(a, b, ...)
#endif // NDEBUG

#endif
