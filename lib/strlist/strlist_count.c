#include "../strlist.h"

size_t
strlist_count(const strlist* sl) {
  size_t p, l = 0;
  for(p = 0; p < sl->sa.len; ++p) {
    if(sl->sa.s[p] == sl->sep) ++l;
  }
  return l;
}
