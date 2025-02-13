#include "../utf8.h"

int
wctou8(char* out, wchar_t w) {
  /* Unicode Table 3-5. UTF-8 Bit Distribution
  Unicode                     1st Byte 2nd Byte 3rd Byte 4th Byte
  00000000 0xxxxxxx           0xxxxxxx
  00000yyy yyxxxxxx           110yyyyy 10xxxxxx
  zzzzyyyy yyxxxxxx           1110zzzz 10yyyyyy 10xxxxxx
  000uuuuu zzzzyyyy yyxxxxxx  11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
  */

  if(!(w & ~0x7f)) {
    *out++ = w & 0x7f;
    *out++ = '\0';
    return 1;
  }

  if(!(w & ~0x7ff)) {
    *out++ = ((w >> 6) & 0x1f) | 0xc0;
    *out++ = (w & 0x3f) | 0x80;
    *out++ = '\0';
    return 2;
  }

  if(!(w & ~0xffff)) {
    *out++ = ((w >> 12) & 0x0f) | 0xe0;
    *out++ = ((w >> 6) & 0x3f) | 0x80;
    *out++ = (w & 0x3f) | 0x80;
    *out++ = '\0';
    return 3;
  }

  if(!(w & ~0x1fffff)) {
    *out++ = ((w >> 18) & 0x07) | 0xf0;
    *out++ = ((w >> 12) & 0x3f) | 0x80;
    *out++ = ((w >> 6) & 0x3f) | 0x80;
    *out++ = (w & 0x3f) | 0x80;
    *out++ = '\0';
    return 4;
  }

  return -1;
}
