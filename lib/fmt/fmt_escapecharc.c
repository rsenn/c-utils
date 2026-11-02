#include "../fmt.h"
#include <sys/types.h>

size_t
fmt_escapecharc(char* dest, int arg) {
  char c;
  unsigned int ch = arg;

  if(ch > 0xff) { /* \uXXXX or \UXXXXXXXX, as in libowfat */
    if(dest) {
      unsigned int i;

      dest[0] = '\\';
      if(ch > 0xffff) {
        dest[1] = 'U';
        dest[2] = fmt_tohex((ch >> 28) & 0xf);
        dest[3] = fmt_tohex((ch >> 24) & 0xf);
        dest[4] = fmt_tohex((ch >> 20) & 0xf);
        dest[5] = fmt_tohex((ch >> 16) & 0xf);
        i = 6;
      } else {
        dest[1] = 'u';
        i = 2;
      }
      dest[i] = fmt_tohex((ch >> 12) & 0xf);
      dest[i + 1] = fmt_tohex((ch >> 8) & 0xf);
      dest[i + 2] = fmt_tohex((ch >> 4) & 0xf);
      dest[i + 3] = fmt_tohex(ch & 0xf);
    }

    return ch > 0xffff ? 10 : 6;
  }

  switch(ch) {
    case '\a': c = 'a'; goto doescape;
    case '\b': c = 'b'; goto doescape;
    case 0x1b: c = 'e'; goto doescape;
    case '\f': c = 'f'; goto doescape;
    case '\n': c = 'n'; goto doescape;
    case '\r': c = 'r'; goto doescape;
    case '\t': c = 't'; goto doescape;
    case '\v': c = 'v'; goto doescape;
    case '\\':
      c = '\\';

    doescape:

      if(dest) {
        dest[0] = '\\';
        dest[1] = c;
      }
      return 2;
    default:

      if(dest) {
        dest[0] = '\\';
        fmt_oct3(dest + 1, ch & 0xff);
      }
      return 4;
  }
}
