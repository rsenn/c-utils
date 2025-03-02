#include "../fmt.h"

size_t
fmt_escapecharquotedprintableutf8(char* dest, int c) {
  char buf[FMT_UTF8];
  unsigned int ch = c;

  size_t i, o, j = fmt_utf8(buf, ch);

  if(!dest)
    return j * 3;

  for(i = o = 0; i < j; ++i)
    o += fmt_escapecharquotedprintable(dest + o, (unsigned char)buf[i]);
  return o;
}
