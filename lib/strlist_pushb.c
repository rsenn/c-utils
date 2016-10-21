#include "strlist.h"
#include "byte.h"

int
strlist_pushb(strlist* sl, const char* s, size_t n) {
  if(stralloc_readyplus(&sl->sa, n + 2)) {
    byte_copy(&sl->sa.s[sl->sa.len], n + 1, s);
    sl->sa.len += n + 1;
    sl->sa.s[sl->sa.len] = '\0';
    return 1;
  }
  return 0;
}
