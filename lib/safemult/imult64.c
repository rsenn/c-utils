#include "../safemult.h"

#if SAFEMULT_NO_INLINE
#if defined(__GNUC__) && (__GNUC__ >= 5)

#include "../uint64.h"

int
imult64(int64 a, int64 b, int64* c) {
  return !__builtin_mul_overflow(a, b, c);
}

#else

#if defined(__x86_64__) && defined(__OPTIMIZE__) && defined(__GNUC__) && !defined(__clang__)

/* WARNING: this only works if compiled with -fomit-frame-pointer */
int
imult64(int64 a, int64 b, int64* c) {
  asm volatile("xchgq %rdx,%rsi\n"
               "movq %rdi,%rax\n"
               "imulq %rdx\n"
               "jc 1f\n" /* overflow */
               "movq %rax,(%rsi)\n"
               "xorq %rax,%rax\n"
               "inc %rax\n"
               "ret\n"
               "1:\n"
               "xorq %rax,%rax\n"
               /* the closing ret is renerated by gcc */
  );
}

#else

#if defined(HAVE_INT128)

int
imult64(int64 a, int64 b, int64* c) {
  __int128_t x = ((__int128_t)a) * b;

  if((*c = (int64)x) != x)
    return 0;
  return 1;
}

#else

int
imult64(int64 a, int64 b, int64* c) {
  int neg = (a < 0);
  uint64 d;

  if(neg)
    a = -a;

  if(b < 0) {
    neg ^= 1;
    b = -b;
  }

  if(!umult64(a, b, &d))
    return 0;

  if(d > (uint64)0x7fffffffffffffff + neg)
    return 0;
  *c = (neg ? -d : d);
  return 1;
}

#endif

#endif

#endif
#endif /* SAFEMULT_NO_INLINE */
