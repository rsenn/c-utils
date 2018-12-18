#include "../stralloc.h"
#include "../ucs.h"

size_t
stralloc_utf8_to_latin1(stralloc* out, const char* s, size_t len) {
  const char* x = s;
  ssize_t i, n = len;
  char* p;

  stralloc_readyplus(out, n);

  p = &out->s[out->len];

  while(n > 0) {
    i = scan_latin1_utf8(x, n, p);

    x += i;
    n -= i;

    ++p;
  }

  n = p - &out->s[out->len];
  out->len += n;
  return n;
}
