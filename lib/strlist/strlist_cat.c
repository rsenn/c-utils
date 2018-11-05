#include "../strlist.h"

int
strlist_cat(strlist* sl, const strlist* l) {
  size_t i, n = sl->sa.len;
  
  if(n && sl->sa.s[n - 1] != sl->sep)
    stralloc_catc(&sl->sa, sl->sep);

  stralloc_catb(&sl->sa, l->sa.s, l->sa.len);
  
  for(i = n; i < sl->sa.len; ++i) {
    if(sl->sa.s[i] == l->sep)
      sl->sa.s[i] = sl->sep;
  }
  return 1;
}
