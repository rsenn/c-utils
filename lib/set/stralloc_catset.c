#include "../set.h"
#include "../str.h"
#include "../stralloc.h"

size_t
stralloc_catset(stralloc* sa, const set_t* set, const char* separator) {
  char* x;
  size_t i = 0, n, slen = str_len(separator);
  set_foreach(set, x, n) {
    if(i++ > 0) {
      stralloc_catb(sa, separator, slen);
      i += slen;
    }
    stralloc_catb(sa, x, n);
    i += n;
  }
  return i;
}
