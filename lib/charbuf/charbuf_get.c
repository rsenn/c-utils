#include "../charbuf.h"
#include "../buffer.h"

int
charbuf_get(charbuf* b) {
  int ret;

  if(b->eof || b->err) {
    ret = -1;
  } else if(b->p) {
    b->p = 0;
  } else {
    b->ch = '\0';
    if((ret = b->op(b->fd, &b->ch, 1, b) <= 0)) {
      if(ret == 0)
        b->eof = 1;
      else if(ret < 0)
        b->err = 1;
    }
  }

#ifdef DEBUG_CHARBUF
  buffer_puts(buffer_2, "charbuf_get '");
  buffer_putc(buffer_2, b->ch);
  buffer_puts(buffer_2, "' eof=");
  buffer_putulong(buffer_2, b->eof ? 1 : 0);
  buffer_puts(buffer_2, " err=");
  buffer_putulong(buffer_2, b->err ? 1 : 0);
  buffer_puts(buffer_2, " ch='");
  buffer_putc(buffer_2, b->ch);
  buffer_puts(buffer_2, "' p=");
  buffer_putulong(buffer_2, b->p ? 1 : 0);
  buffer_putnlflush(buffer_2);
#endif

  return (int)(unsigned int)(unsigned char)b->ch;
}
