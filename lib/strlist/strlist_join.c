#include "../byte.h"
#include "../stralloc.h"
#include "../strlist.h"

void
strlist_join(const strlist* sl, stralloc* sa, char delim) {
  const char* x, *y;
  size_t i = 0, n;

  for(x = sl->sa.s, y=sl->sa.s+sl->sa.len; x < y; ) {
//  strlist_foreach(sl, x, n) {
//  
   n = byte_chr(x, y-x, sl->sep);

    if(i)
      stralloc_catc(sa, delim);
    stralloc_catb(sa, x, n);
    ++i;
  }
}
