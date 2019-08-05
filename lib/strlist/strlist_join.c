#include "../byte.h"
#include "../stralloc.h"
#include "../strlist.h"

void
strlist_join(const strlist* sl, stralloc* sa, char delim) {
  const char* x;
  size_t i = 0, n;
  strlist_foreach(sl, x, n) {
    if(i)
      stralloc_catc(sa, delim);
    stralloc_catb(sa, x, n);
    ++i;
  }
}

