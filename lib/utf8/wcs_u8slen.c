#include "../utf8.h"

size_t
wcs_u8slen(const wchar_t* in) {
  size_t len = 0;
  wchar_t w;

  while((w = *in++)) {
    if(w < 0 || w > 0x10ffff)
      len += 6;
    else if(!(w & ~0x7f))
      len += 1;
    else if(!(w & ~0x7ff))
      len += 2;
    else if(!(w & ~0xffff))
      len += 3;
    else
      len += 4;
  }

  return len;
}
