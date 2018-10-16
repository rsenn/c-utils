#include "../strlist.h"

int
strlist_cat(strlist* sl, const strlist* l) {
  size_t i, n = sl->sa.len;
  if(sl->sa.len) stralloc_catc(&sl->sa, sl->sep);
  stralloc_catb(&sl->sa, l->sa.s, l->sa.len);
  stralloc_readyplus(&sl->sa, 1);
  sl->sa.s[sl->sa.len] = sl->sep;
  for(i = n; i < sl->sa.len; ++i) {
    if(sl->sa.s[i] == l->sep) sl->sa.s[i] = sl->sep;
  }
  return 1;
}
