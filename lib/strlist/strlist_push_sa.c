#include "../byte.h"
#include "../str.h"
#include "../strlist.h"

int
strlist_push_sa(strlist* sl, const stralloc* sa) {
  if(stralloc_readyplus(&sl->sa, sa->len + 1)) {
    if(sl->sa.len) {
      sl->sa.s[sl->sa.len] = sl->sep;
      ++sl->sa.len;
    }
    byte_copy(&sl->sa.s[sl->sa.len], sa->len, sa->s);
    sl->sa.len += sa->len;
    return 1;
  }
  return 0;
}
