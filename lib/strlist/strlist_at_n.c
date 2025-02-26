#include "../strlist.h"

char*
strlist_at_n(const strlist* sl, size_t i, size_t* n) {
  size_t p, l = 0;

  for(p = 0; p < sl->sa.len; ++p) {
    if(l == i) {
      *n = byte_chr(&sl->sa.s[p], sl->sa.len - p, sl->sep);
      return &sl->sa.s[p];
    }

    if(sl->sa.s[p] == sl->sep)
      ++l;
  }
  *n = 0;
  return 0;
}
