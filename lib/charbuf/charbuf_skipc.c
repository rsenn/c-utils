#include "../charbuf_internal.h"
#include "../fmt.h"

ssize_t
charbuf_skipc(charbuf* b, uint8* ch) {
  ssize_t ret;

  for(;;) {
    if(b->p) {
      if(b->chrs[0] == '\n') {
        b->loc.column = 0;
        b->loc.line++;
      } else {
        b->loc.column++;
      }
      if(ch)
        *ch = b->chrs[0];
      if(--b->p > 0)
        byte_copy(b->chrs, b->p, b->chrs + 1);
      b->offset++;
      ret = 1;

    } else if((ret = charbuf_stubborn_read(b, 1)) > 0) {
      b->p = ret;
      continue;
    }
    break;
  }

#ifdef DEBUG_CHARBUF
  if(charbuf_debug) {
    buffer_puts(buffer_2, "charbuf_skipc *ch=");
    if(*ch > 0x20 || *ch == 0x0a || *ch == 0x0d || *ch == 9) {
      buffer_putc(buffer_2, '\'');
      if(*ch == 0x0a || *ch == 0x0d || *ch == 0x09)
        buffer_puts(buffer_2, *ch == '\n' ? "\\n" : *ch == '\r' ? "\\r" : "\\t");
      else
        buffer_putc(buffer_2, *ch);
      buffer_putc(buffer_2, '\'');
    } else if(*ch > 0 && *ch < 0x20 && !(*ch == 0x0a || *ch == 0x0d || *ch == 9)) {
      buffer_puts(buffer_2, "x");
      buffer_putxlong0(buffer_2, *ch, 2);
    } else {
      int pad = 3 - fmt_8long(0, *ch);
      while(pad-- > 0) buffer_putc(buffer_2, '0');
      buffer_put8long(buffer_2, *ch);
    }
    charbuf_dump(b, buffer_2);
    buffer_puts(buffer_2, charbuf_colors ? ", \x1b[1;30mret\x1b[m=\x1b[1;36m" : ", ret=");
    buffer_putlonglong(buffer_2, ret);
    if(charbuf_colors)
      buffer_puts(buffer_2, "\x1b[m");
    buffer_putnlflush(buffer_2);
  }
#endif
  return ret;
}
