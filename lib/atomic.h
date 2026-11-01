#ifndef ATOMIC_H
#define ATOMIC_H

/* Atomic operations on machine words (size_t), API-compatible with
 * libowfat's CAS.h.  The header is named atomic.h because lib/cas.h
 * already exists (the internal __CAS macros) and file names that differ
 * only in case cannot live in one directory on Windows or macOS.  Source
 * written for libowfat: replace #include <CAS.h> with #include "atomic.h".
 *
 * Differences to libowfat 0.32:
 *   - atomic_add_return really returns the new value with the GCC
 *     builtins (the original forgot the `return').
 *   - atomic_inc/dec/add/or/and take volatile pointers throughout.
 */

#include <stddef.h>

#if defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)

/* if (*x == oldval) { *x = newval; return 1; } else return 0; */
static inline int
compare_and_swap(volatile size_t* x, size_t oldval, size_t newval) {
  return __sync_bool_compare_and_swap(x, oldval, newval);
}
/* return *x += val; */
static inline size_t
atomic_add_return(volatile size_t* x, size_t val) {
  return __sync_add_and_fetch(x, val);
}
static inline void
atomic_add(volatile size_t* x, size_t val) {
  (void)__sync_add_and_fetch(x, val);
}
static inline void
atomic_or(volatile size_t* x, size_t val) {
  (void)__sync_or_and_fetch(x, val);
}
static inline void
atomic_and(volatile size_t* x, size_t val) {
  (void)__sync_and_and_fetch(x, val);
}

#elif defined(_WIN32)

#include <windows.h>

static __inline int
compare_and_swap(volatile size_t* x, size_t oldval, size_t newval) {
  return (size_t)InterlockedCompareExchangePointer((void* volatile*)x, (void*)newval, (void*)oldval) == oldval;
}
static __inline size_t
atomic_add_return(volatile size_t* x, size_t val) {
  size_t y;
  do
    y = *x;
  while(!compare_and_swap(x, y, y + val));
  return y + val;
}
static __inline void
atomic_add(volatile size_t* x, size_t val) {
  (void)atomic_add_return(x, val);
}
static __inline void
atomic_or(volatile size_t* x, size_t val) {
  size_t y;
  do
    y = *x;
  while(!compare_and_swap(x, y, y | val));
}
static __inline void
atomic_and(volatile size_t* x, size_t val) {
  size_t y;
  do
    y = *x;
  while(!compare_and_swap(x, y, y & val));
}

#else
#error atomic.h: compiler not supported, add the primitives here
#endif

#define atomic_inc(x) atomic_add((x), 1)
#define atomic_dec(x) atomic_add((x), (size_t)-1)
#define atomic_inc_return(x) atomic_add_return((x), 1)
#define atomic_dec_return(x) atomic_add_return((x), (size_t)-1)

/* Optimization barrier */
#if defined(__GNUC__) || defined(__clang__)
#define barrier() __asm__ __volatile__("" : : : "memory")
#define mb() __sync_synchronize()
#define rmb() __sync_synchronize()
#define wmb() __sync_synchronize()
#define smp_mb__before_atomic_dec() barrier()
#define smp_mb__after_atomic_dec() barrier()
#define smp_mb__before_atomic_inc() barrier()
#define smp_mb__after_atomic_inc() barrier()
#endif

#endif
