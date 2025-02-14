#include "../utf8.h"

size_t
wc_u8len(const wchar_t w) {
  if(!(w & ~0x7f))
    return 1;

  if(!(w & ~0x7ff))
    return 2;

  if(!(w & ~0xffff))
    return 3;

  if(!(w & ~0x1fffff))
    return 4;

  return 0; /* error */
}
