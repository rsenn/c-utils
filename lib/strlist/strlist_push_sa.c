#include "../strlist.h"
#include "../byte.h"
#include "../str.h"

int
strlist_push_sa(strlist* sl, const stralloc* sa) {
  if(stralloc_readyplus(&sl->sa, sa->len + 1)) {
    byte_copy(&sl->sa.s[sl->sa.len], sa->len, sa->s);
    sl->sa.len += sa->len;
    sl->sa.s[sl->sa.len] = '\0';
    ++sl->sa.len;
    return 1;
  }
  return 0;
}
