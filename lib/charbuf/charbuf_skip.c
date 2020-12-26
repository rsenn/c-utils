#include "../charbuf.h"
#include "../buffer.h"

int
charbuf_skip(charbuf* b) {
  int ret;

#ifdef DEBUG_CHARBUF
  buffer_puts(buffer_2, "charbuf_skip p=");
  buffer_putulong(buffer_2, b->p);
  buffer_puts(buffer_2, " ch='");
  buffer_putc(buffer_2, b->ch);
  buffer_puts(buffer_2, "'");
  buffer_putnlflush(buffer_2);
#endif

  if(b->p) {
    b->p = 0;
    ret = 1;
  }

  if((ret = b->op(b->fd, &b->ch, 1, b) <= 0)) {
    if(ret == 0)
      b->eof = 1;
    else if(ret < 0)
      b->err = 1;
  } else {
    b->p = 0;
  }
  b->ch = '\0';

  return ret;
}
