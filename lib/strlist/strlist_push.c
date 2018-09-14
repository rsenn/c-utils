#include "../byte.h"
#include "../str.h"
#include "../strlist.h"

int
strlist_push(strlist* sl, const char* s) {
  size_t n = str_len(s);
  if(stralloc_readyplus(&sl->sa, n + 1)) {
    if(sl->sa.len) {
      if(sl->sa.s[sl->sa.len - 1] != sl->sep) {
        sl->sa.s[sl->sa.len] = sl->sep;
        ++sl->sa.len;
      }
    }
    if(n == 0) {
      sl->sa.s[sl->sa.len++] = sl->sep;
    } else {
      byte_copy(&sl->sa.s[sl->sa.len], n, s);
      sl->sa.len += n;
    }
    return 1;
  }
  return 0;
}
