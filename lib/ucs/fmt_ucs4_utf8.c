#include "../ucs.h"

/* fmt_ucs4_utf8 writes c as UTF-8 and returns the length, 0 if c is not a
 * scalar value. dest may be NULL to only measure. */
size_t
fmt_ucs4_utf8(char* dest, uint32 c) {
  size_t len = c < 0x80 ? 1 : c < 0x800 ? 2 : c < 0x10000 ? 3 : 4;

  if(c > 0x10ffff || (c >= 0xd800 && c <= 0xdfff))
    return 0;

  if(dest) {
    if(len == 1)
      dest[0] = (char)c;
    else {
      size_t i;

      for(i = len; --i;) {
        dest[i] = (char)(0x80 | (c & 0x3f));
        c >>= 6;
      }
      dest[0] = (char)(((0xff00 >> len) & 0xff) | c);
    }
  }
  return len;
}
