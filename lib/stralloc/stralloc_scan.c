#include "../stralloc.h"

size_t
stralloc_scan(stralloc* out, const stralloc* in, size_t (*scan_function)(const char*, char*)) {
  size_t i, n;
  stralloc_zero(out);

  for(i = 0; i < in->len; ++out->len) {
    if(!stralloc_readyplus(out, 1))
      return 0;

    n = scan_function(&in->s[i], stralloc_end(out));
    if(n == 0) { /* a scan function that consumes nothing would loop forever */
      return out->len;
    }
    i += n;
  }

  return out->len;
}
