#include "../stralloc.h"

size_t
stralloc_scan(stralloc* out, const stralloc* in, size_t (*scan_function)(const char*, char*)) {
  size_t i;
  stralloc_zero(out);
  for(i = 0; i < in->len; ++out->len) {
    if(!stralloc_readyplus(out, 1))
      return 0;
    i += scan_function(&in->s[i], &out->s[out->len]);
  }
  return out->len;
}

