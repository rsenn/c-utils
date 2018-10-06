#include "../strlist.h"
#include "../byte.h"

int
strlist_prependb(strlist* sl, const char* b, size_t n) {
  if(stralloc_readyplus(&sl->sa, n + 2)) {
    byte_copyr(&sl->sa.s[n + 1], sl->sa.len, sl->sa.s);
    byte_copy(sl->sa.s, n, b);
    sl->sa.s[n] = sl->sep;
    sl->sa.len += n + 1;
    return 1;
  }
  return 0;
}
