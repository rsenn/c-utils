#include "../byte.h"

typedef struct stralloc_s {
  char* s;
  size_t len, a;
} stralloc;

void stralloc_zero(stralloc*);
int stralloc_readyplus(stralloc*, size_t);

size_t
byte_scan(const char* in, size_t in_len, stralloc* out, size_t (*scan_function)(const char*, char*)) {
  size_t i;
  stralloc_zero(out);
  for(i = 0; i < in_len; ++out->len) {
    if(!stralloc_readyplus(out, 1))
      return 0;
    i += scan_function(&in[i], &out->s[out->len]);
  }
  return out->len;
}
