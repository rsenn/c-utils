#include "strlist.h"
#include "byte.h"
#include "str.h"

int
strlist_push(strlist* sl, const char* s) {
  size_t n = str_len(s) + 1;
  if(stralloc_readyplus(&sl->sa, n)) {
    byte_copy(&sl->sa.s[sl->sa.len], n, s);
    sl->sa.len += n;
    return 1;
  }
  return 0;
}
