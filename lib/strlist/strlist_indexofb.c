#include "../byte.h"
#include "../strlist.h"
#include "../uint64.h"

int64
strlist_indexofb(strlist* sl, const char* x, size_t n) {
  size_t p;
  const char* s = sl->sa.s;
  for(p = 0; p < sl->sa.len;) {
    size_t l = byte_chr(&sl->sa.s[p], sl->sa.len - p, sl->sep);
    if((l == 0 && p + 1 == sl->sa.len)) break;

    if((n == l && byte_equal(&sl->sa.s[p], l, x))) return p;

    p += l + 1;
  }
  return -1;
}
