#include "../ucs.h"

/* ISO 8859-1 is the first 256 code points of Unicode, so the conversion is the
 * identity. (The conversion table that used to be here was all zeros: every
 * character from 0x80 on became U+0000.) */
wchar_t
ucs_from_latin1_char(int ch) {
  return (unsigned char)ch;
}

/* returns 0 for a code point that Latin-1 cannot represent */
char
ucs_to_latin1_char(wchar_t ch) {
  if((unsigned long)ch <= 0xff)
    return (char)ch;
  return 0;
}
