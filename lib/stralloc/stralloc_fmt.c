#include "../stralloc.h"

size_t
stralloc_fmt(const stralloc* in, stralloc* out, size_t (*fmt_function)(char*,unsigned int ch)) {
  stralloc_zero(out);
  for(size_t i = 0; i < in->len; ++i) {
    if(!stralloc_readyplus(out, 10))
      return 0;
    out->len += fmt_function(&out->s[out->len], in->s[i]);
  }
  stralloc_trunc(out, out->len);
  return out->len;
}
