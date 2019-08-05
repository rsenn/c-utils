#include "../safemult.h"

#if SAFEMULT_NO_INLINE
#if defined(__GNUC__) && (__GNUC__ >= 5)

#include "../uint32.h"
#include "../uint64.h"

int
umult32(uint32 a, uint32 b, uint32* c) {
  return !__builtin_mul_overflow(a, b, c);
}

#else

int
umult32(uint32 a, uint32 b, uint32* c) {
  uint64 x = (uint64)a * b;
  if(x > 0xffffffff)
    return 0;
  *c = x & 0xffffffff;
  return 1;
}

#endif
#endif /* SAFEMULT_NO_INLINE */

