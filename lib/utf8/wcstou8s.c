#include "../utf8.h"

size_t
wcstou8s(char* u8, const wchar_t* in, size_t count) {
  size_t clen = 0;
  wchar_t w;
  const int len = wcsu8slen(in);

  if(NULL == u8)
    return (size_t)len;

  while((w = *in++)) {
    const int ulen = wcu8len(w);

    if(ulen >= 0) {
      if((clen + wcu8len(w)) <= count) {
        clen += wctou8(u8, w);
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
