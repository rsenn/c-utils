#include "../stralloc.h"

size_t
stralloc_scan(const stralloc* in, stralloc* out, size_t (*scan_function)(const char*, char*)) {
  stralloc_zero(out);
  for(size_t i = 0; i < in->len; ++out->len) {
    if(!stralloc_readyplus(out, 1)) return 0;
    i += scan_function(&in->s[i], &out->s[out->len]);
  }
  return out->len;
}
