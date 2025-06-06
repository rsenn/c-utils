#include "../byte.h"
#include "../str.h"
#include "../strlist.h"

int
strlist_push_sa(strlist* sl, const stralloc* sa) {
  if(sl->sa.len) {
    if(sl->sa.s[sl->sa.len - 1] != sl->sep)
      stralloc_catc(&sl->sa, sl->sep);
  }

  if(sa->len) {
    stralloc_cat(&sl->sa, sa);
    return 1;
  }
  return 0;
}
