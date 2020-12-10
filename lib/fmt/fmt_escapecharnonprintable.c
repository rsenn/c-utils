#include "../fmt.h"
#include <ctype.h>

void
fmt_oct3(char* dest, unsigned char w) {
  dest[2] = (char)((w & 7) + '0');
  w >>= 3;
  dest[1] = (char)((w & 7) + '0');
  w >>= 3;
  dest[0] = (char)((w & 7) + '0');
}

size_t
fmt_escapecharnonprintable(char* dest, int c) {
  char ch = c;
  size_t r;

  if(isprint(ch) || (unsigned char)ch > 'z') {
    if(dest)
      *dest = c;
    return 1;
  }
  r = c == '\0' ? 2 : 4;
  if(dest) {
    *dest++ = '\\';
    if(c)
      fmt_oct3(dest, c);
    else
      *dest = '0';
  }
  return r;
}