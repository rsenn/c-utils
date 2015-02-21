#include "strlist.h"
#include "byte.h"
#include "str.h"

int
strlist_append(strlist* sl, const char* s) {
  size_t n = str_len(s);
  if(stralloc_readyplus(&sl->sa, n + 2)) {
    byte_copy(&sl->sa.s[sl->sa.len], n + 1, s);
    sl->sa.len += n + 1;
    sl->sa.s[sl->sa.len] = '\0';
    return 1;
  }
  return 0;
}
