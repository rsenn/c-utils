#include "../byte.h"
#include "../stralloc.h"

size_t
byte_scan(const char* in,
          size_t in_len,
          stralloc* out,
          size_t (*scan_function)(const char*, char*)) {
  size_t i;
  stralloc_zero(out);
  for(i = 0; i < in_len; ++out->len) {
    if(!stralloc_readyplus(out, 10))
      return 0;
    i += scan_function(&in[i], &out->s[out->len]);
  }
  return out->len;
}
