#include "../ucs.h"

/* scan_utf8_ucs4 decodes one code point from src[n] into *dest and returns
 * the bytes consumed, 0 if empty, truncated, overlong or not a scalar value. */
size_t
scan_utf8_ucs4(const char* src, size_t n, uint32* dest) {
  const unsigned char* s = (const unsigned char*)src;
  uint32 c;
  size_t len, i;

  if(!n)
    return 0;

  if(*s < 0x80) {
    *dest = *s;
    return 1;
  }

  if(*s >= 0xc2 && *s <= 0xdf) {
    len = 2;
    c = *s & 0x1f;
  } else if(*s >= 0xe0 && *s <= 0xef) {
    len = 3;
    c = *s & 0x0f;
  } else if(*s >= 0xf0 && *s <= 0xf4) {
    len = 4;
    c = *s & 0x07;
  } else
    return 0;

  if(n < len)
    return 0;

  for(i = 1; i < len; ++i) {
    if((s[i] & 0xc0) != 0x80)
      return 0;
    c = (c << 6) | (s[i] & 0x3f);
  }

  if((len == 3 && c < 0x800) || (len == 4 && (c < 0x10000 || c > 0x10ffff)) || (c >= 0xd800 && c <= 0xdfff))
    return 0;

  *dest = c;
  return len;
}
