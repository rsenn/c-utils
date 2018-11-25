#include "../byte.h"
#include "../strlist.h"

void
strlist_fromb(strlist* sl, const char* x, size_t n, const char* delim) {
  size_t len = str_len(delim);
  stralloc_zero(&sl->sa);

  if(x == NULL) return;

  for(; n;) {
    size_t i = byte_finds(x, n, delim);

    strlist_pushb(sl, x, i); 

    if(i + len > n)
      break;
      
    i += len;

    x += i;
    n -= i;
  }
}
