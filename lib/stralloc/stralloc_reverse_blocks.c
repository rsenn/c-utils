
/* ISC license. */

#include "../byte.h"
#include "../stralloc.h"
#include <stdlib.h>

void
stralloc_reverse_blocks(stralloc* sa, size_t size) {
  size_t n = sa->len / (size << 1);
  size_t i = 0;

#ifdef __GNUC__
  char tmp[size];
#elif defined(HAVE_ALLOCA)
  char* tmp = alloca(size);
#else
  char* tmp = malloc(size);
#endif
  for(; i < n; i++) {
    byte_copy(tmp, size, sa->s + i * size);
    byte_copy(sa->s + i * size, size, sa->s + (2 * n - 1 - i) * size);
    byte_copy(sa->s + (2 * n - 1 - i) * size, size, tmp);
  }

#if !defined(__GNUC__) && !defined(HAVE_ALLOCA)
  free(tmp);
#endif
}

