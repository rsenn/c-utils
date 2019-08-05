#include "../byte.h"
#include "../str.h"
#include "../strlist.h"

int
strlist_append_sa(strlist* sl, const stralloc* sa) {
  if(stralloc_readyplus(&sl->sa, sa->len + 2)) {
    byte_copy(&sl->sa.s[sl->sa.len], sa->len + 1, sa->s);
    sl->sa.len += sa->len + 1;
    sl->sa.s[sl->sa.len] = '\0';
    return 1;
  }
  return 0;
}

