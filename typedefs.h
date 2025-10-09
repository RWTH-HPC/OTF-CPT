#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <atomic>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

namespace __otfcpt {

extern std::atomic<uint32_t> current_verbosity;

typedef unsigned long uptr;
typedef signed long sptr;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

#if defined(_MSC_VER)
#define ALWAYS_INLINE __forceinline
// FIXME(timurrrr): do we need this on Windows?
#define ALIAS(x)
#define ALIGNED(x) __declspec(align(x))
#define FORMAT(f, a)
#define NOINLINE __declspec(noinline)
#define NORETURN __declspec(noreturn)
#define THREADLOCAL __declspec(thread)
#define LIKELY(x) (x)
#define UNLIKELY(x) (x)
#define PREFETCH(x) /* _mm_prefetch(x, _MM_HINT_NTA) */ (void)0
#define WARN_UNUSED_RESULT
#else // _MSC_VER
#define ALWAYS_INLINE inline __attribute__((always_inline))
#define ALIAS(x) __attribute__((alias(SANITIZER_STRINGIFY(x))))
// Please only use the ALIGNED macro before the type.
// Using ALIGNED after the variable declaration is not portable!
#define ALIGNED(x) __attribute__((aligned(x)))
#define FORMAT(f, a) __attribute__((format(printf, f, a)))
#define NOINLINE __attribute__((noinline))
#define NORETURN __attribute__((noreturn))
#define THREADLOCAL __thread
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#if defined(__i386__) || defined(__x86_64__)
// __builtin_prefetch(x) generates prefetchnt0 on x86
#define PREFETCH(x) __asm__("prefetchnta (%0)" : : "r"(x))
#else
#define PREFETCH(x) __builtin_prefetch(x)
#endif
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#endif // _MSC_VER

#if !defined(_MSC_VER) || defined(__clang__)
#define UNUSED __attribute__((unused))
#define USED __attribute__((used))
#else
#define UNUSED
#define USED
#endif

#ifdef _MSC_VER
#define MSC_PREREQ(version) (_MSC_VER >= (version))
#else
#define MSC_PREREQ(version) 0
#endif

#if !defined(_MSC_VER) || defined(__clang__) || MSC_PREREQ(1900)
#define NOEXCEPT noexcept
#else
#define NOEXCEPT throw()
#endif

#if __has_cpp_attribute(clang::fallthrough)
#define FALLTHROUGH [[clang::fallthrough]]
#elif __has_cpp_attribute(fallthrough)
#define FALLTHROUGH [[fallthrough]]
#else
#define FALLTHROUGH
#endif

void NORETURN Die();

void NORETURN CheckFailed(const char *file, int line, const char *cond, u64 v1,
                          u64 v2);

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
    if (UNLIKELY(!(v1 op v2)))                                                 \
      CheckFailed(__FILE__, __LINE__, "(" #c1 ") " #op " (" #c2 ")", v1, v2);  \
  } while (false) /**/

#define CHECK(a) CHECK_IMPL((a), !=, 0)
#define CHECK_EQ(a, b) CHECK_IMPL((a), ==, (b))
#define CHECK_NE(a, b) CHECK_IMPL((a), !=, (b))
#define CHECK_LT(a, b) CHECK_IMPL((a), <, (b))
#define CHECK_LE(a, b) CHECK_IMPL((a), <=, (b))
#define CHECK_GT(a, b) CHECK_IMPL((a), >, (b))
#define CHECK_GE(a, b) CHECK_IMPL((a), >=, (b))

#if SANITIZER_DEBUG
#define DCHECK(a) CHECK(a)
#define DCHECK_EQ(a, b) CHECK_EQ(a, b)
#define DCHECK_NE(a, b) CHECK_NE(a, b)
#define DCHECK_LT(a, b) CHECK_LT(a, b)
#define DCHECK_LE(a, b) CHECK_LE(a, b)
#define DCHECK_GT(a, b) CHECK_GT(a, b)
#define DCHECK_GE(a, b) CHECK_GE(a, b)
#else
#define DCHECK(a)
#define DCHECK_EQ(a, b)
#define DCHECK_NE(a, b)
#define DCHECK_LT(a, b)
#define DCHECK_LE(a, b)
#define DCHECK_GT(a, b)
#define DCHECK_GE(a, b)
#endif

#if SANITIZER_WINDOWS
#if SANITIZER_IMPORT_INTERFACE
#define SANITIZER_INTERFACE_ATTRIBUTE __declspec(dllimport)
#else
#define SANITIZER_INTERFACE_ATTRIBUTE __declspec(dllexport)
#endif
#define SANITIZER_WEAK_ATTRIBUTE
#define SANITIZER_WEAK_IMPORT
#elif SANITIZER_GO
#define SANITIZER_INTERFACE_ATTRIBUTE
#define SANITIZER_WEAK_ATTRIBUTE
#define SANITIZER_WEAK_IMPORT
#else
#define SANITIZER_INTERFACE_ATTRIBUTE __attribute__((visibility("default")))
#define SANITIZER_WEAK_ATTRIBUTE __attribute__((weak))
#if SANITIZER_APPLE
#define SANITIZER_WEAK_IMPORT extern "C" __attribute((weak_import))
#else
#define SANITIZER_WEAK_IMPORT extern "C" SANITIZER_WEAK_ATTRIBUTE
#endif // SANITIZER_APPLE
#endif // SANITIZER_WINDOWS

#define SANITIZER_INTERFACE_WEAK_DEF(ReturnType, Name, ...)                    \
  extern "C" SANITIZER_INTERFACE_ATTRIBUTE SANITIZER_WEAK_ATTRIBUTE ReturnType \
  Name(__VA_ARGS__)

#define VPrintf(level, ...)                                                    \
  do {                                                                         \
    if ((uptr)Verbosity() >= (level))                                          \
      printf(__VA_ARGS__);                                                     \
  } while (0)

} // namespace __otfcpt
#endif
