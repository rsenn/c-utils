#include "../fmt.h"
#include "../uint32.h"
#include <sys/types.h>

void
fmt_hex2(char* dest, unsigned char w) {
  dest[2] = fmt_hexchars[w & 0x0f];
  w >>= 4;
  dest[1] = fmt_hexchars[w & 0x0f];
  dest[0] = 'x';
}

size_t
fmt_escapecharcx(char* dest, int chr) {
  char c;
  uint32 ch = chr;

  if(ch > 0xff)
    return 0;

  switch(ch) {
    case '\a': c = 'a'; goto doescape;
    case '\b': c = 'b'; goto doescape;
    case '\e': c = 'e'; goto doescape;
    case '\f': c = 'f'; goto doescape;
    case '\n': c = 'n'; goto doescape;
    case '\r': c = 'r'; goto doescape;
    case '\t': c = 't'; goto doescape;
    case '\v': c = 'v'; goto doescape;
    case '\\':
      c = '\\';

    doescape: {
      if(dest) {
        dest[0] = '\\';
        dest[1] = c;
      }

      return 2;
  }
    default: {
      if(dest) {
        dest[0] = '\\';
        fmt_hex2(dest + 1, ch & 0xff);
      }
     
      return 4;
    }
  }
}
