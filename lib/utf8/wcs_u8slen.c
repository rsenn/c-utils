#include "../utf8.h"

int
wcs_u8slen(const wchar_t* in) {
  int len = 0;
  wchar_t w;

  while((w = *in++)) {
    if(!(w & ~0x7f))
      len += 1;
    else if(!(w & ~0x7ff))
      len += 2;
    else if(!(w & ~0xffff))
      len += 3;
    else if(!(w & ~0x1fffff))
      len += 4;
    else /* error: add width of null character entity &#x00; */
      len += 6;
  }

  return len;
}
