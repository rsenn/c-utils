#include "../fmt.h"
#include "../uint32.h"
#include <ctype.h>

size_t
fmt_escapecharcontrol(char* dest, int c) {
  uint32 ch = c;
  size_t r;

  if(!iscntrl(ch) && ch >= 0x20) {
    if(dest)
      *dest = c;
    return 1;
  }

  r = ch == '\0' || ch == '\r' || ch == '\n' || ch == '\t' ? 2 : 4;

  if(dest) {
    *dest++ = '\\';

    if(ch == '\r')
      *dest = 'r';
    else if(ch == '\n')
      *dest = 'n';
    else if(ch == '\t')
      *dest = 't';
    else if(ch)
      fmt_8long(dest, ch);
    else
      *dest = '0';
  }
  return r;
}
