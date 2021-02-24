#include "../charbuf.h"
#include "../buffer.h"

ssize_t
charbuf_peek(charbuf* b) {
  ssize_t ret;
  for(;;) {
    if(b->p) {
      ret = (unsigned int)(unsigned char)b->chrs[0];
      break;
    }
    if((ret = charbuf_stubborn_read(b, 1)) <= 0)
      break;
    b->p = ret;
  }

#ifdef DEBUG_CHARBUF
  buffer_puts(buffer_2, "charbuf_peek '");
  buffer_putc(buffer_2, b->chrs[0]);
  buffer_puts(buffer_2, "' eof=");
  buffer_putulong(buffer_2, b->eof ? 1 : 0);
  buffer_puts(buffer_2, " err=");
  buffer_putulong(buffer_2, b->err ? 1 : 0);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);
#endif

  return ret;
}