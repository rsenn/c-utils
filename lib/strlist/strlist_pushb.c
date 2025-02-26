#include "../byte.h"
#include "../strlist.h"

int
strlist_pushb(strlist* sl, const char* s, size_t n) {
  if(sl->sa.len)
    stralloc_catc(&sl->sa, sl->sep);

  if(n)
    stralloc_catb(&sl->sa, s, n);
  /*  n = sl->sa.len;
    stralloc_catc(&sl->sa, sl->sep);
    sl->sa.len = n;*/
  return 1;
}
