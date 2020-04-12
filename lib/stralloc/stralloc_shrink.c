#include <stdlib.h>
/* ISC license. */

#include "../stralloc.h"

int
stralloc_shrink(stralloc* sa) {
  if(sa->a > sa->len) {
    if(!(sa->s = alloc_re(sa->s, sa->len)))
      return 0;
    sa->a = sa->len;
  }
  return 1;
}
