#include "../byte.h"
#include "../stralloc.h"

size_t
byte_fmt(const char* in, size_t in_len, stralloc* out, size_t (*fmt_function)(char*, unsigned int ch)) {
  stralloc_zero(out);

  for(size_t i = 0; i < in_len; ++i) {
    if(!stralloc_readyplus(out, 10))
      return 0;

    out->len += fmt_function(stralloc_end(out), in[i]);
  }

  stralloc_trunc(out, out->len);
  return out->len;
}
