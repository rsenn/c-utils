#include "../byte.h"
#include "../strlist.h"

int
strlist_pushb(strlist* sl, const char* s, size_t n) {
  if(sl->sa.len) {
    if(sl->sa.s[sl->sa.len - 1] != sl->sep)
      stralloc_catc(&sl->sa, sl->sep);
  }
  if(n)
    stralloc_catb(&sl->sa, s, n);

  stralloc_readyplus(&sl->sa, 1);
    sl->sa.s[sl->sa.len] = sl->sep;

  if(!n)
    ++sl->sa.len;

  return 1;
}