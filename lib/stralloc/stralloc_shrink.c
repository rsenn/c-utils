#include <stdlib.h>
/* ISC license. */

#include "../stralloc.h"

int
stralloc_shrink(stralloc* sa) {
  if(sa->a > sa->len) {
    if(!(sa->s = realloc(sa->s, sa->len))) return 0;
    sa->a = sa->len;
  }
  return 1;
}
