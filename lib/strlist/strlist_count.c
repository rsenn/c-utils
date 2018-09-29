#include "../strlist.h"

size_t
strlist_count(const strlist* sl) {
  size_t p, l = 1;

  if(sl->sa.len == 0) return 0;

  for(p = 0; p < sl->sa.len; ++p) {
    if(sl->sa.s[p] == sl->sep) {
      if(p + 1 == sl->sa.len) return l;
      ++l;
    }
  }
  return l;
}
