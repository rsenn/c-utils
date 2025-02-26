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

    if(ch)
      fmt_8long(dest, ch);
    else
      *dest = '0';
  }

  return r;
}
