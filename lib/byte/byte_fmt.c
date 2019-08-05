#include "../byte.h"

typedef struct stralloc_s { char* s; size_t len, a; } stralloc;

void stralloc_zero(stralloc*);
int stralloc_readyplus(stralloc*, size_t);
int stralloc_trunc(stralloc*, size_t);

size_t
byte_fmt(const char* in, size_t in_len, stralloc* out, size_t (*fmt_function)(char*, unsigned int ch)) {
  size_t i;
  stralloc_zero(out);
  for(i = 0; i < in_len; ++i) {
    if(!stralloc_readyplus(out, 10)) return 0;
    out->len += fmt_function(&out->s[out->len], in[i]);
  }
  stralloc_trunc(out, out->len);
  return out->len;
}

