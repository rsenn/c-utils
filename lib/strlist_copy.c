#include "strlist.h"

int
strlist_copy (strlist * sl, const strlist* l) {
  size_t i, n = strlist_count(sl);
  int ret = stralloc_cat((stralloc*)sl, (const stralloc*)l);
  stralloc_0((stralloc*)sl);
  return ret;
}

  