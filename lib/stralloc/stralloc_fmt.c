#include "../stralloc.h"

size_t
stralloc_fmt(stralloc* out, const char* in, size_t in_len, size_t (*fmt_function)(char*, int)) {
  size_t i;
  for(i = 0; i < in_len; ++i) {
    size_t n = ((size_t(*)(char*, int, int))fmt_function)(0, (unsigned int)(unsigned char)in[i], 0);
    if(!stralloc_readyplus(out, n + 1))
      return 0;
    out->len += ((size_t(*)(char*, int, int))fmt_function)(&out->s[out->len],
                                                           (unsigned int)(unsigned char)in[i],
                                                           0);
  }
  return out->len;
}
