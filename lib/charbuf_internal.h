#include "uint64.h"
#include "buffer.h"
#include "charbuf.h"
#include "fmt.h"

#include <ctype.h>
#include <alloca.h>

#define CHARBUF_SEP "\x1b[m  "
#define CHARBUF_EQ " " //"\x1b[38;5;233m="
#define CHARBUF_BLACK "\x1b[0;30m"
#define CHARBUF_GRAY "\x1b[1;30m"
#define CHARBUF_GRAY20 "\x1b[38;5;233m"
#define CHARBUF_CYAN "\x1b[1;36m"
#define CHARBUF_GREEN "\x1b[0;32m"
#define CHARBUF_YELLOW "\x1b[0;33m"
#define CHARBUF_NC "\x1b[m"

extern int charbuf_debug;
extern int charbuf_colors;

#define charbuf_colorstr(str, color, out) \
  buffer_putm_internal( out, charbuf_colors ? COLOR_NC : "", charbuf_colors ? color : "", str, charbuf_colors ? COLOR_NC : "", 0);
#define charbuf_dumpname(lbl, out) charbuf_colorstr(lbl, CHARBUF_GRAY, out)
#define charbuf_dumplabel(lbl, out) \
  do { \
    charbuf_dumpname(lbl, out); \
    charbuf_colorstr("=", CHARBUF_GRAY20); \
  } while(0)

static inline void
charbuf_dumpret(ssize_t ret, buffer* out) {
  charbuf_dumplabel(CHARBUF_SEP "ret", out);
  buffer_putlonglong(out, ret);
}

static inline void
charbuf_dumpchars(uint8* chrs, size_t n, buffer* out) {
  size_t i = 0;
  char buf[256];

  buf[i++] = '\'';
  for(; n > 0; chrs++, n--) {
    int c = *chrs;

    if(isprint(c) || c == ' ') {

      buf[i++] = c;
    } else if(isspace(c)) {
      buf[i++] = '\'';
      i += fmt_escapecharc(&buf[i], c);
      buf[i++] = '\'';
    } else {
      buf[i++] = '0';
      int pad = 3 - fmt_8long(0, c);
      buf[i++] = '\\';
      while(pad-- > 0) buf[i++] = '0';
      i += fmt_8long(&buf[i], c);
    }
  }
  buf[i++] = '\'';
  buffer_put(out, buf, i);
}

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