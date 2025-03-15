#include "../fmt.h"
#include "../uint32.h"
#include <ctype.h>

size_t
fmt_escapecharnonprintable(char* dest, int c) {
  uint32 ch = c;
  size_t r;

  if((isprint(ch) || ch > 'z') && ch >= 0x20) {
    if(dest)
      *dest = c;
    return 1;
  }

  r = ch == '\0' ? 2 : 4;

  if(dest) {
    *dest++ = '\\';

    switch(ch) {
      case '\n': *dest = 'n'; break;
      case '\r': *dest = 'r'; break;
      case '\t': *dest = 't'; break;
      case '\b': *dest = 'b'; break;
      case '\0': *dest = '0'; break;
      default: fmt_8long(dest, ch); break;
    }
  }

  return r;
}
