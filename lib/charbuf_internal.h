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
#define CHARBUF_GRAY40 "\x1b[38;5;235m"
#define CHARBUF_BLUE "\x1b[0;34m"
#define CHARBUF_LIGHT_BLUE "\x1b[1;34m"
#define CHARBUF_MARINE "\x1b[0;36m"
#define CHARBUF_CYAN "\x1b[1;36m"
#define CHARBUF_GREEN "\x1b[0;32m"
#define CHARBUF_LIGHT_GREEN "\x1b[1;32m"
#define CHARBUF_RED "\x1b[0;31m"
#define CHARBUF_LIGHT_RED "\x1b[1;31m"
#define CHARBUF_YELLOW "\x1b[0;33m"
#define CHARBUF_LIGHT_YELLOW "\x1b[1;33m"
#define CHARBUF_MAGENTA "\x1b[0;35m"
#define CHARBUF_LIGHT_MAGENTA "\x1b[1;35m"
#define CHARBUF_NC "\x1b[m"

#define CHARBUF_PROPSEP ", "
#define CHARBUF_PREFIX "."

extern int charbuf_debug;
extern int charbuf_colors;

ssize_t charbuf_stubborn_read(charbuf*, size_t max);


#define charbuf_colorstr(str, color, out)                                                                              \
  buffer_putm_internal(                                                                                                \
      out, charbuf_colors ? CHARBUF_NC : "", charbuf_colors ? color : "", str, charbuf_colors ? CHARBUF_NC : "", 0);
#define charbuf_dumpname(lbl, out) charbuf_colorstr(lbl, CHARBUF_GRAY, out)
#define charbuf_dumplabel(lbl, out)                                                                                    \
  do {                                                                                                                 \
    charbuf_colorstr(CHARBUF_PREFIX, "\x1b[38;2;0;120;120m", out);                                                     \
    charbuf_colorstr(lbl, CHARBUF_GRAY, out);                                                                          \
    charbuf_colorstr("=", "\x1b[38;2;0;100;100m", out);                                                                \
  } while(0)

#define charbuf_dumpint64(lbl, val, out)                                                                               \
  do {                                                                                                                 \
    char buf[128];                                                                                                     \
    size_t n;                                                                                                          \
    charbuf_dumplabel(lbl, out);                                                                                       \
    n = fmt_longlong(buf, (val));                                                                                      \
    buf[n++] = '\0';                                                                                                   \
    charbuf_colorstr(buf, CHARBUF_YELLOW, out);                                                                        \
  } while(0)

static inline void
charbuf_dumpchar(int c, buffer* out, int pad) {
  char buf[64];
  int quote = pad < 0;
  if(pad < 0)
    pad = -pad;

  size_t l, i = 0;
  if(isprint(c) || c == ' ') {
    if(quote)
      buf[i++] = '\'';

    buf[i++] = c;
    if(quote)
      buf[i++] = '\'';
  } else if(isspace(c) /*&& !iscntrl(c)*/) {
    if(quote)
      buf[i++] = '\'';
    i += fmt_escapecharc(&buf[i], c);
    if(quote)
      buf[i++] = '\'';
  } else {
    buf[i++] = '\\';
    buf[i++] = 'u';
    int pad = 4 - fmt_xlong(0, c);
    while(pad-- > 0) buf[i++] = '0';
    i += fmt_xlong(&buf[i], c);
  }

  if(i < pad) {
    l = (size_t)(pad - i) / 2 - 1;
    buffer_putnspace(out, l);
  }
  buffer_put(out, buf, i);
  if(l + i < pad) {
    size_t r = pad - (l + i);
    buffer_putnspace(out, r);
  }
}

static inline void
charbuf_dumpchars(uint8* chrs, size_t n, buffer* out, int pad) {
  size_t i = 0;
  buffer_puts(buffer_2, "'");
  for(i = 0; i < n; i++) {
    if(chrs[i] == '\'')
      buffer_puts(out, "\\'");
    else
    charbuf_dumpchar(chrs[i], out, pad);
   }
  buffer_puts(buffer_2, "'");
}
