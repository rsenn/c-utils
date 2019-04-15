#ifdef __DMC__
#define InterlockedCompareExchange(p, n, o) InterlockedCompareExchange((void**)p, (void*)n, (void*)o)
#endif

#if (defined(__i386__) || defined(__x86_64__)) && defined(__TINYC__)
int __inline__ __sync_val_compare_and_swap( volatile unsigned int *ptr, int cmp, int new) {
  unsigned char ret;
  __asm__ __volatile__ (
  " lock\n"
  " cmpxchgl %2,%1\n"
  " sete %0\n"
  : "=q" (ret), "=m" (*ptr)
  : "r" (new), "m" (*ptr), "a" (cmp)
  : "memory");
  return (int) ret;
}
#endif

#ifdef __arm__
typedef long(__kernel_cmpxchg_t)(long oldval, long newval, long* ptr);
static inline long
__CAS(long* ptr, long oldval, long newval) {
  long actual_oldval, fail;

  while(1) {
    actual_oldval = *ptr;

    if(__builtin_expect(oldval != actual_oldval, 0))
      return actual_oldval;

    fail = (*(__kernel_cmpxchg_t*)0xffff0fc0)(actual_oldval, newval, ptr);

    if(__builtin_expect(!fail, 1))
      return oldval;
  }
}
#elif defined(__STDC__) && (__STDC_VERSION__ >= 201112L) && !defined(__EMSCRIPTEN__)

#include <stdatomic.h>

static inline long
__atomic_compare_and_swap(long* ptr, long oldval, long newval) {
#if defined(__ORANGEC__)
  atomic_compare_swap(ptr, &oldval, newval);
#else
  __atomic_compare_exchange_n(ptr, &oldval, newval, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#endif
  return oldval;
}

#define __CAS __atomic_compare_and_swap
#define __CAS_PTR __atomic_compare_and_swap

#elif defined(__dietlibc__)

#include <sys/atomic.h>

#elif WINDOWS_NATIVE || (defined(__CYGWIN__) && __MSYS__ == 1) || defined(__POCC__)
#include <windows.h>
#define __CAS(ptr, oldval, newval) InterlockedCompareExchange(ptr, newval, oldval)
#else
#define __CAS(ptr, oldval, newval) __sync_val_compare_and_swap(ptr, oldval, newval)
#endif

#ifndef __CAS
  #define __CAS __sync_val_compare_and_swap
#endif
#ifndef __CAS_PTR
  #define __CAS_PTR __CAS
#endif

