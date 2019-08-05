#include <stdlib.h>
/* ISC license. */

#include "../stralloc.h"
#include <errno.h>
#include <string.h>

int
stralloc_ready_tuned(stralloc* sa, size_t n, size_t base, size_t a, size_t b) {
  size_t t;
  if(!b)
    return (errno = EINVAL, 0);
  t = n + base + a * n / b;
  if(t < n)
    return (errno = ERANGE, 0);
  if(!sa->s) {
    sa->s = malloc(t);
    if(!sa->s)
      return 0;
    sa->a = t;
  } else if(n > sa->a) {
    if(!(sa->s = realloc(sa->s, t)))
      return 0;
    sa->a = t;
  }
  return 1;
}

