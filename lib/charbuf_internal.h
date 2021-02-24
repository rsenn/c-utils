#include "uint64.h"
#include "buffer.h"
#include "charbuf.h"
#include "fmt.h"
#include <ctype.h>

#define CHARBUF_SEP "\x1b[m  "
#define CHARBUF_EQ " " //"\x1b[38;5;233m="
#define CHARBUF_BLACK "\x1b[0;30m"
#define CHARBUF_GRAY "\x1b[1;30m"
#define CHARBUF_CYAN "\x1b[1;36m"
#define CHARBUF_GREEN "\x1b[0;32m"
#define CHARBUF_YELLOW "\x1b[0;33m"
#define CHARBUF_NC "\x1b[m"

static inline void
charbuf_dumpchar(int c, buffer* out) {
  char buf[64];
  size_t l, i = 0;
  if(isprint(c) || c == ' ') {
    buf[i++] = '\'';

    buf[i++] = c;
    buf[i++] = '\'';
  } else if(isspace(c) /*&& !iscntrl(c)*/) {
    buf[i++] = '\'';
    i += fmt_escapecharc(&buf[i], c);
    buf[i++] = '\'';
  } else {
    buf[i++] = '0';
    int pad = 3 - fmt_8long(0, c);
    while(pad-- > 0) buf[i++] = '0';
    i += fmt_8long(&buf[i], c);
  }

  if(i < 6) {
    l = ((6 - i) + 1) / 2;
    buffer_putnspace(out, l);
  }
  buffer_put(out, buf, i);
  if(l + i < 6) {
    size_t r = 6 - l - i;
    buffer_putnspace(out, r);
  }
  return;

  if(c > 0x20 || c == 0x0a || c == 0x0d || c == 9) {
    buffer_putc(out, '\'');
    if(c == 0x0a || c == 0x0d || c == 0x09)
      buffer_puts(out, c == '\n' ? "\\n" : c == '\r' ? "\\r" : "\\t");
    else
      buffer_putc(out, c);
    buffer_putc(out, '\'');
  } else if(c > 0 && c < 0x20 && !(c == 0x0a || c == 0x0d || c == 9)) {
    buffer_puts(out, "x");
    buffer_putxlong0(out, c, 2);
  } else {
    int pad = 3 - fmt_8long(0, c);
    while(pad-- > 0) buffer_putc(out, '0');
    buffer_put8long(out, c);
  }
}