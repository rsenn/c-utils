#include "../charbuf.h"
#include "../buffer.h"

ssize_t
charbuf_skip(charbuf* b) {
  ssize_t ret;

  for(;;) {
    if(b->p) {
      if(b->chrs[0] == '\n') {
        b->loc.column = 0;
        b->loc.line++;
      } else {
        b->loc.column++;
      }
      ret = (unsigned int)(unsigned char)b->chrs[0];

      if(--b->p > 0)
        byte_copy(b->chrs, b->p, b->chrs + 1);

    } else if((ret = charbuf_stubborn_read(b, 1)) > 0) {
      b->p = ret;
    } else {
      break;
    }
  }

#ifdef DEBUG_CHARBUF
  buffer_puts(buffer_2, "charbuf_skip ");
  if(ret < 0x20) {
    buffer_puts(buffer_2, "x");
    buffer_putxlong0(buffer_2, ret, 2);
  } else if(ret >= 0) {
    buffer_putc(buffer_2, '\'');
    buffer_putc(buffer_2, ret);
    buffer_putc(buffer_2, '\'');
  }
  buffer_puts(buffer_2, " p=");
  buffer_putulong(buffer_2, b->p);
  buffer_puts(buffer_2, " a=");
  buffer_putulong(buffer_2, b->a);
  buffer_puts(buffer_2, " eof=");
  buffer_putulong(buffer_2, b->eof);
  buffer_puts(buffer_2, " err=");
  buffer_putulong(buffer_2, b->err);
  buffer_puts(buffer_2, " loc=");
  buffer_putulong(buffer_2, b->loc.line);
  buffer_puts(buffer_2, ":");
  buffer_putulong(buffer_2, b->loc.column);
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
