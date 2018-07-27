#include "../stralloc.h"

size_t
byte_fmt_pred(const char* in, size_t in_len, stralloc* out, size_t (*fmt_function)(char*,unsigned int ch), int (*pred)(int)) {
  size_t i;
  stralloc_zero(out);
  for(i = 0; i < in_len; ++i) {
    if(!stralloc_readyplus(out, 10))
      return 0;

    if(pred(in[i]))
      out->len += fmt_function(&out->s[out->len], in[i]);
    else
      out->s[out->len++] = in[i];
  }
  return out->len;
}
