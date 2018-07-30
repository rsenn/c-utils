#include "../stralloc.h"

size_t
stralloc_fmt(stralloc* out, const char* in, size_t in_len, size_t (*fmt_function)()) {
  size_t i;
  stralloc_zero(out);
  for(i = 0; i < in_len; ++i) {
    if(!stralloc_readyplus(out, 10)) return 0;
    out->len += fmt_function(&out->s[out->len], in[i]);
  }
  stralloc_trunc(out, out->len);
  return out->len;
}
