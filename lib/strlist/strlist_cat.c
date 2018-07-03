#include "../strlist.h"

int
strlist_cat (strlist * sl, const strlist* l) {
  return stralloc_catb((stralloc*)sl, l->sa.s, l->sa.len);
}


