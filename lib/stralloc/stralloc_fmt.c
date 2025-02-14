#include "../stralloc.h"
#include "../uint32.h"

size_t
stralloc_fmt(stralloc* out,
             const char* in,
             size_t len,
             size_t (*fn)(char*, int)) {
  size_t i;

  for(i = 0; i < len; ++i) {
    size_t n = ((size_t(*)(char*, uint32, void*))
                    fn)(0, (unsigned int)(unsigned char)in[i], 0);

    if(!stralloc_readyplus(out, n + 1))
      return 0;

    out->len += ((size_t(*)(char*, uint32, void*))fn)(
        &out->s[out->len], (unsigned int)(unsigned char)in[i], 0);
  }

  return out->len;
}
