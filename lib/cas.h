#ifdef __DMC__
#define InterlockedCompareExchange(p, n, o) \
  InterlockedCompareExchange((void**)p, (void*)n, (void*)o)
#endif

#ifdef __TINYC__
#define BLAH 1
#endif

#if defined(__dietlibc__)

#include <sys/atomic.h>
#elif defined(__STDC__) && (__STDC_VERSION__ >= 201112L) && \
    !defined(__EMSCRIPTEN__)

#include <stdatomic.h>

static __inline long
__atomic_compare_and_swap(volatile long* ptr, long oldval, long newval) {
#if defined(__ORANGEC__)
  atomic_compare_swap(ptr, &oldval, newval);
#else
  __atomic_compare_exchange_n(
      ptr, &oldval, newval, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#endif
  return oldval;
}

#define __CAS __atomic_compare_and_swap
#define __CAS_PTR __atomic_compare_and_swap

#elif(defined(__i386__) || defined(__x86_64__)) && \
    (defined(__TINYC__) || defined(TCC) || defined(__GNUC__) || \
     USE_INLINE_COMPARE_AND_SWAP) && \
    !defined(__BORLANDC__)
//#warning x86

#ifdef __TINYC__
//#warning TCC
#endif

#ifdef __GNUC__
//#warning GNUC
#undef __sync_val_compare_and_swap
//#define __sync_val_compare_and_swap  __CAS
#else
#endif

#undef __CAS

#if defined(__TINYC__) || __GNUC__ == 3
#warning __TINYC__
static inline uint64_t
__compare_and_swap(uint64_t* ptr, uint64_t new_val, uint64_t old_val) {
  uint64_t out;

  // newline after `lock' for the work around of apple's gas(?) bug.
  asm volatile("lock cmpxchgq %2,%1"
               : "=a"(out), "+m"(*ptr)
               : "q"(new_val), "0"(old_val)
               : "cc");

  return out;
}
#endif

#define __CAS __compare_and_swap
static inline long
__sync_val_compare_and_swap(long* ptr, long cmp, long new) {
  unsigned long prev;
  __asm__ volatile("lock;"
#if defined(__x86_64__)
                   "cmpxchgq %1, %2;"
#else
                   "cmpxchgl %1, %2;"
#endif
                   : "=a"(prev)
                   : "q"(new), "m"(*ptr), "a"(cmp)
                   : "memory");
  return prev;
}
#elif(defined(__aarch64__) || defined(__ARM_ARCH_8A__)) && \
    !defined(__TINYC__)

static inline long
__atomic_compare_and_swap(void* ptr, long old, long new) {
  long oldval;
  long* lptr = ptr;
  unsigned long res;
  __asm__ __volatile__("1: ldxr %1, %2\n"
                       " cmp %1, %3\n"
                       " b.ne 2f\n"
                       " stxr %w0, %4, %2\n"
                       " cbnz %w0, 1b\n"
                       "2:"
                       : "=&r"(res), "=&r"(oldval), "+Q"(ptr)
                       : "Ir"(old), "r"(new)
                       : "cc");
  return oldval;
}

#define __sync_val_compare_and_swap __atomic_compare_and_swap
#elif defined(__arm__)
typedef long(_cmpxchg_t)(long oldval, long newval, long* ptr);
static inline long
__CAS(long* ptr, long oldval, long newval) {
  long actual_oldval, fail;

  for(;;) {
    actual_oldval = *ptr;

    if(__builtin_expect(oldval != actual_oldval, 0))
      return actual_oldval;

    fail = (*(_cmpxchg_t*)0xffff0fc0)(actual_oldval, newval, ptr);

    if(__builtin_expect(!fail, 1))
      return oldval;
  }
}

#elif WINDOWS_NATIVE || (defined(__CYGWIN__) && __MSYS__ == 1) || \
    defined(__POCC__)
#include <windows.h>
#define __CAS(ptr, oldval, newval) \
  InterlockedCompareExchange((LONG*)ptr, (LONG)newval, (LONG)oldval)
#else
#define __CAS(ptr, oldval, newval) \
  __sync_val_compare_and_swap(ptr, oldval, newval)
#endif

#ifndef __CAS
#define __CAS __sync_val_compare_and_swap
#endif
#ifndef __CAS_PTR
#define __CAS_PTR __CAS
#endif
