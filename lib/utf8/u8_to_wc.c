#include "../utf8.h"
#include <assert.h>

/* Decodes the UTF-8 sequence at u8 into *out and returns the number of bytes it
 * takes, 1 to 4. Returns 0 if there is none: at the end of the string, for a
 * byte that cannot start a sequence, a sequence whose continuation bytes are
 * missing or wrong (also at the end of the string), an overlong encoding, a
 * surrogate (U+D800..U+DFFF) or a value above U+10FFFF. *out is not touched
 * then. */
size_t
u8_to_wc(wchar_t* out, const char* u8) {
  const unsigned char* s = (const unsigned char*)u8;
  size_t len;
  unsigned long w;

  assert(out);

  if((len = u8_len(u8, 1)) < 1)
    return 0;

  if(len == 1) {
    *out = s[0];
    return 1;
  }

  if(len == 2) {
    if((s[1] & 0xc0) != 0x80)
      return 0;

    w = ((unsigned long)(s[0] & 0x1f) << 6) | (s[1] & 0x3f);

    if(w < 0x80) /* overlong */
      return 0;
  } else if(len == 3) {
    if((s[1] & 0xc0) != 0x80 || (s[2] & 0xc0) != 0x80)
      return 0;

    w = ((unsigned long)(s[0] & 0x0f) << 12) | ((unsigned long)(s[1] & 0x3f) << 6) | (s[2] & 0x3f);

    if(w < 0x800 || (w >= 0xd800 && w <= 0xdfff))
      return 0;
  } else {
    if((s[1] & 0xc0) != 0x80 || (s[2] & 0xc0) != 0x80 || (s[3] & 0xc0) != 0x80)
      return 0;

    w = ((unsigned long)(s[0] & 0x07) << 18) | ((unsigned long)(s[1] & 0x3f) << 12) | ((unsigned long)(s[2] & 0x3f) << 6) |
        (s[3] & 0x3f);

    if(w < 0x10000 || w > 0x10ffff)
      return 0;
  }

  *out = (wchar_t)w;
  return len;
}
