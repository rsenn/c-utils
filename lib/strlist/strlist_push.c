#include "../strlist.h"
#include "../byte.h"
#include "../str.h"

int
strlist_push(strlist* sl, const char* s) {
  size_t n = str_len(s);
  if(stralloc_readyplus(&sl->sa, n + 1)) {
    byte_copy(&sl->sa.s[sl->sa.len], n, s);
    sl->sa.len += n;
    sl->sa.s[sl->sa.len] = '\0';
    ++sl->sa.len;
    return 1;
  }
  return 0;
}
