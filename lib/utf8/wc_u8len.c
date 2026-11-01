#include "../utf8.h"

size_t
wc_u8len(const wchar_t w) {
  /* Unicode ends at U+10FFFF */
  if(w < 0 || w > 0x10ffff)
    return 0;

  if(!(w & ~0x7f))
    return 1;

  if(!(w & ~0x7ff))
    return 2;

  if(!(w & ~0xffff))
    return 3;

  return 4;
}
