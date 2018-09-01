#include "../byte.h"
#include "../strlist.h"

stralloc
strlist_at_sa(const strlist* sl, size_t i) {
  stralloc sa;
  size_t p, l = 0;
  stralloc_init(&sa);
  for(p = 0; p < sl->sa.len; ++p) {
    if(l == i) {
      sa.s = &sl->sa.s[p];
      sa.len = byte_chr(&sl->sa.s[p], sl->sa.len - p, sl->sep);
      sa.a = 0;
      return sa;
    }

    if(sl->sa.s[p] == sl->sep) ++l;
  }
  return sa;
}
