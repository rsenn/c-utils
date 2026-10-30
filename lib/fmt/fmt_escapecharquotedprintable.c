#include "../fmt.h"

size_t
fmt_escapecharquotedprintable(char* dest, int c) {
  unsigned int ch = c;

  if(ch > 0xff)
    return 0;

  if(dest) {
    dest[0] = '=';
    dest[2] = "0123456789ABCDEF"[ch & 0xf];
    dest[1] = "0123456789ABCDEF"[(ch >> 4) & 0xf];
  }
  return 3;
}
