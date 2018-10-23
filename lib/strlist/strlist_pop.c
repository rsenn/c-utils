#include "../strlist.h"

int
strlist_pop(strlist* sl) {
  size_t n = strlist_count(sl);

  if(n >= 1) {
    const char* p = strlist_at(sl, n - 1);
    stralloc_trunc(&sl->sa, p - sl->sa.s);
    return 1;
  }
  return 0;
}