#include "../fmt.h"

size_t
fmt_escapecharquotedprintable(char* dest, int c) {
  unsigned int ch = c;

  if(ch > 0xff)
    return 0;

  if(dest) {
    dest[0] = '=';
    dest[2] = fmt_tohex(ch & 0xf);
    ch >>= 4;
    dest[1] = fmt_tohex(ch & 0xf);
  }
  return 3;
}
