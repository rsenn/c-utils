#include "../byte.h"
#include "../stralloc.h"
#include "../strlist.h"

void
strlist_joinq(const strlist* sl, stralloc* sa, char delim, char quote) {
  const char* x;
  size_t i = 0, n;

  strlist_foreach(sl, x, n) {
    int q = byte_chr(x, n, delim) < n;

    if(i)
      stralloc_catc(sa, delim);

    if(q)
      stralloc_catc(sa, quote);
    stralloc_catb(sa, x, n);

    if(q)
      stralloc_catc(sa, quote);
    ++i;
  }
}
