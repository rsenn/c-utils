#include "strlist.h"
#include "byte.h"
#include "str.h"

int
strlist_pushsa(strlist* sl, const stralloc* sa) {
  if(stralloc_readyplus(&sl->sa, sa->n + 2)) {
    byte_copy(&sl->sa.s[sl->sa.len], sa->n + 1, sa->s);
    sl->sa.len += sa->n + 1;
    sl->sa.s[sl->sa.len] = '\0';
    return 1;
  }
  return 0;
}
