#include "../ucs.h"

size_t
stralloc_utf8_to_latin1(const stralloc* in, stralloc* out) {
  const char* x = in->x;
  ssize_t i, n = in->len;
  char* p;

  stralloc_readyplus(out, n);
  
  p = &out->x[out->len];


  while(n > 0) {
    stralloc_readyplus
    i = scan_latin1_utf8(x, n, p);

    x += i;
    n -= i;

   ++p;
  }

  n = p - &out->x[out->len];
  out->len += n;
  return n;
}
