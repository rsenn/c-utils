#include <stdlib.h>
#include "../alloc.h"
#include "../byte.h"

int
alloc_re(void* ptr, size_t m, size_t n) {
  char **x = ptr, *y;

#ifdef DJB_STRICT
  if(!(y = alloc(n)))
    return 0;

  byte_copy(y, m, *x);
  alloc_free(*x);

  *x = y;
#else
  y = (char*)realloc(*x, n);

  if(!y)
    return 0;

  /*if(n > m)
    byte_zero(&y[m], n - m);*/

  *x = y;
#endif
  return 1;
}
