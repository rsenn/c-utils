#include "../charbuf.h"
#include "../buffer.h"

ssize_t
charbuf_peekc(charbuf* b, unsigned char* ch) {
  ssize_t ret;

  for(;;) {
    if(b->p) {
      *ch = b->chrs[b->p - 1];
      return ret;
    }

    if((ret = charbuf_stubborn_read(b, 1)) > 0) {
      b->p += ret;
      continue;
    }
    break;
  }

#ifdef DEBUG_CHARBUF_
  buffer_puts(buffer_2, "charbuf_peekc '");
  buffer_putc(buffer_2, b->ch);
  buffer_puts(buffer_2, "' eof=");
  buffer_putulong(buffer_2, b->eof ? 1 : 0);
  buffer_puts(buffer_2, " err=");
  buffer_putulong(buffer_2, b->err ? 1 : 0);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, b->err ? 1 : 0);
  buffer_putnlflush(buffer_2);
#endif

  return ret;
}
