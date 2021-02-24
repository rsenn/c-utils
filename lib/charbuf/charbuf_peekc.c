#include "../buffer.h"
#include "../charbuf.h"

ssize_t
charbuf_peekc(charbuf* b, unsigned char* ch) {
  ssize_t ret;

  for(;;) {
    if(b->p) {
      *ch = (unsigned int)(unsigned char)b->chrs[0];
      ret = 1;
      break;
    } else if((ret = charbuf_stubborn_read(b, 1)) > 0) {
      b->p = ret;
      continue;
    }

    break;
  }

#ifdef DEBUG_CHARBUF
  if(charbuf_debug) {
    buffer_puts(buffer_2, "charbuf_peekc *ch=");
    if(*ch > 0x20 || *ch == 0x0a || *ch == 0x0d || *ch == 9) {
      buffer_putc(buffer_2, '\'');
      if(*ch == 0x0a || *ch == 0x0d || *ch == 0x09)
        buffer_puts(buffer_2, *ch == '\n' ? "\\n" : *ch == '\r' ? "\\r" : "\\t");
      else
        buffer_putc(buffer_2, *ch);
      buffer_putc(buffer_2, '\'');
    } else if(*ch > 0 && *ch < 0x20) {
      buffer_puts(buffer_2, "x");
      buffer_putxlong0(buffer_2, *ch, 2);
    } else {
      buffer_putlong(buffer_2, *ch);
    }
    charbuf_dump(b, buffer_2);
  }
#endif

  return ret;
}
