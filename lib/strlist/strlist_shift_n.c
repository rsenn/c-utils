#include "../strlist.h"

int
strlist_shift_n(strlist* sl, size_t i) {
  size_t n = strlist_at(sl, i) - sl->sa.s;

  if(n < sl->sa.len) {
    byte_copy(sl->sa.s, sl->sa.len - n, &sl->sa.s[n]);
    sl->sa.len -= n;
    stralloc_shrink(&sl->sa);
    return 1;
  }

  return 0;
}
