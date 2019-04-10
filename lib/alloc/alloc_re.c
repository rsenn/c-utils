#include <stdlib.h>
#include "../alloc.h"
#include "../byte.h"

int
alloc_re(void** x, size_t m, size_t n) {
  char* y;
#ifdef DJB_STRICT
  y = alloc(n);
  if(!y)
    return 0;
  byte_copy(y, m, *x);
  alloc_free(*x);
  *x = y;
  return 1;
#else
  y = realloc(*x, n);
  if(!y)
    return 0;
  // if(n > m)
  //   byte_zero(&y[m], n - m);
  *x = y;
  return 1;
#endif
}
