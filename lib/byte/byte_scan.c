#include "../stralloc.h"

size_t
byte_scan(const char* in, size_t in_len, stralloc* out, size_t (*scan_function)(const char*, char*)) {
  stralloc_zero(out);
  for(size_t i = 0; i < in_len; ++out->len) {
    if(!stralloc_readyplus(out, 1)) return 0;
    i += scan_function(&in[i], &out->s[out->len]);
  }
  return out->len;
}
