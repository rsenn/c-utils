#include "../stralloc.h"

size_t
stralloc_fmt(stralloc* out, const char* in, size_t in_len, size_t (*fmt_function)(char*, int)) {
  size_t i;
  // stralloc_zero(out);
  for(i = 0; i < in_len; ++i) {
    // size_t n = fmt_function(NULL, (unsigned int)(unsigned char)in[i], 0);
    if(!stralloc_readyplus(out, 10))
      return 0;
    out->len += ((size_t(*)(char*, int, int))fmt_function)(&out->s[out->len], (unsigned int)(unsigned char)in[i], 0);
  }
  // stralloc_trunc(out, out->len);
  return out->len;
}
