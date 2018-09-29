#include "../byte.h"
#include "../strlist.h"

int
strlist_pushb(strlist* sl, const char* s, size_t n) {
  if(stralloc_readyplus(&sl->sa, n + 1)) {
    if(sl->sa.len) stralloc_catb(&sl->sa, &sl->sep, 1);
    stralloc_catb(&sl->sa, s, n);
    stralloc_catc(&sl->sa, sl->sep);
  }
  return 0;
}
