#include "../utf8.h"

size_t
wcs_to_u8s(char* u8, const wchar_t* in, size_t count) {
  size_t clen = 0;
  wchar_t w;
  const size_t len = wcs_u8slen(in);

  if(NULL == u8)
    return (size_t)len;

  while((w = *in++)) {
    const size_t ulen = wc_u8len(w);

    if(ulen >= 0) {
      size_t l = wc_u8len(w);
      if((clen + l) <= count) {
        clen += wc_to_u8(u8, w);
        u8 += ulen;
      } else
        break;
    } else {
      if((clen + 6) <= count) {
        *u8++ = '&';
        *u8++ = '#';
        *u8++ = 'x';
        *u8++ = '0';
        *u8++ = '0';
        *u8++ = ';';
      } else
        break;
    }
  }

  return (size_t)clen;
}
