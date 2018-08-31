#include "../strlist.h"

char*
strlist_at(const strlist* sl, size_t i) {
  size_t p, l = 0;
  for(p = 0; p < sl->sa.len; ++p) {
    if(l == i) return &sl->sa.s[p];

    if(sl->sa.s[p] == sl->sep) ++l;
  }
  return 0;
}
