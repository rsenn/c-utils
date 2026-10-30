#include "../fmt.h"
#include "../uint32.h"
#include <ctype.h>

size_t
fmt_escapecharnonprintable(char* dest, int c) {
  uint32 ch = c;
  size_t r;

  if((ch > 'z' || isprint(ch)) && ch >= 0x20) {
    if(dest)
      *dest = c;
    return 1;
  }

  r = (ch == '\n' || ch == '\r' || ch == '\t' || ch == '\b' || ch == '\0') ? 2 : 4;

  if(dest) {
    *dest++ = '\\';

    switch(ch) {
      case '\n': *dest = 'n'; break;
      case '\r': *dest = 'r'; break;
      case '\t': *dest = 't'; break;
      case '\b': *dest = 'b'; break;
      case '\0': *dest = '0'; break;
      default:
        dest[0] = (char)('0' + ((ch >> 6) & 7));
        dest[1] = (char)('0' + ((ch >> 3) & 7));
        dest[2] = (char)('0' + (ch & 7));
        break;
    }
  }

  return r;
}
