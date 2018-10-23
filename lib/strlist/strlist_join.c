#include "../byte.h"
#include "../stralloc.h"
#include "../strlist.h"

void
strlist_join(const strlist* sl, stralloc* sa, const char* delim) {
  const char* x;
  size_t i = 0, n;
  strlist_foreach(sl, x, n) {
    if(i) stralloc_cats(sa, delim);
    stralloc_catb(sa, x, n);
    ++i;
  }
}
