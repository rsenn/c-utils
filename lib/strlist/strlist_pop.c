#include "../strlist.h"

int
strlist_pop(strlist* sl) {
  size_t n = strlist_count(sl);

  if(n >= 1) {
    const char* p = strlist_at(sl, n - 1);

    sl->sa.len = p - sl->sa.s;

    stralloc_removesuffixb(&sl->sa, &sl->sep, 1);

    return 1;
  }
  return 0;
}