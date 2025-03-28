#include "../stralloc.h"

size_t
stralloc_fmt_pred(stralloc* out, const char* in, size_t in_len, size_t (*fn)(char*, int), int (*pred)(int)) {
  stralloc_zero(out);

  for(size_t i = 0; i < in_len; ++i) {
    if(!stralloc_readyplus(out, 10))
      return 0;

    if(pred(in[i]))
      out->len += ((size_t(*)(char*, int, int))fn)(stralloc_end(out), (unsigned int)(unsigned char)in[i], 0);
    else
      out->s[out->len++] = in[i];
  }

  return out->len;
}
