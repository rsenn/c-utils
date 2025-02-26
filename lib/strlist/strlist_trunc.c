#include "../strlist.h"

int
strlist_trunc(strlist* l, size_t items) {
  size_t n;
  char* x;

  if(items == 0) {
    stralloc_trunc(&l->sa, 0);
    return 0;
  }

  if((x = strlist_at_n(l, items - 1, &n))) {
    n += x - l->sa.s;
    stralloc_trunc(&l->sa, n);
    return items;
  }

  return 0;
}
