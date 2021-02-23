#include "../charbuf.h"
#include "../buffer.h"

int
charbuf_peek(charbuf* b) {
  int ret = -1;

  if(!b->p) {
    b->ch = '\0';

      if((ret = charbuf_stubborn_read(b) <= 0))  
            return ret;

      b->p = 1;
  }
 
  if(b->p)
    ret = (unsigned int)(unsigned char)b->ch;

#ifdef DEBUG_CHARBUF_
  buffer_puts(buffer_2, "charbuf_peek '");
  buffer_putc(buffer_2, b->ch);
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