#include "../fmt.h"
#include "../uint16.h"
#include "../uint32.h"

static void
fmt_hex4(char* dest, uint16 w) {
  dest[3] = fmt_tohex(w & 0xf);
  w >>= 4;
  dest[2] = fmt_tohex(w & 0xf);
  w >>= 4;
  dest[1] = fmt_tohex(w & 0xf);
  w >>= 4;
  dest[0] = fmt_tohex(w & 0xf);
}

size_t
fmt_escapecharjson(char* dest, int c, int quote) {
  size_t n;
  uint32 ch = c;

  if(quote != '"' && quote != '\'')
    quote = '"';

  if(ch != (uint32)(unsigned)quote) {
    if(ch == '\'' || ch == '"') {
      dest[0] = ch;
      return 1;
    }
  }

  switch(ch) {
    case '\b': ch = 'b'; goto simple;
    case '\n': ch = 'n'; goto simple;
    case '\r': ch = 'r'; goto simple;
    case '"':
    case '\\':
    case '/':
    simple:

      if(dest) {
        dest[0] = '\\';
        dest[1] = (char)ch;
      }
      return 2;
  }

  if(ch > 0xffff) {
    if(ch > 0x10ffff)
      return 0; // highest representable unicode codepoint

    if(dest) {
      dest[0] = '\\';
      dest[1] = 'u';
      fmt_hex4(dest + 2, (uint16)(0xd800 | (((ch - 0x10000) >> 10) & 0x3ff)));
      dest += 6;
    }
    ch = (ch & 0x3ff) | 0xdc00;
    n = 6;
  } else
    n = 0;

  if(dest) {
    dest[0] = '\\';
    dest[1] = 'u';
    fmt_hex4(dest + 2, (uint16)ch);
  }
  return n + 6;
}
