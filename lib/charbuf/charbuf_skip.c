#include "../charbuf.h"
#include "../buffer.h"

ssize_t
charbuf_skip(charbuf* b) {
  ssize_t ret;

#ifdef DEBUG_CHARBUF
  buffer_puts(buffer_2, "charbuf_skip ");
  if(b->ch < 0x20) {
    buffer_puts(buffer_2, "x");
    buffer_putxlong0(buffer_2, b->ch, 2);
  } else {
    buffer_putc(buffer_2, '\'');
    buffer_putc(buffer_2, b->ch);
    buffer_putc(buffer_2, '\'');
  }
  buffer_puts(buffer_2, " p=");
  buffer_putulong(buffer_2, b->p ? 1 : 0);
  buffer_puts(buffer_2, " loc=");
  buffer_putulong(buffer_2, b->loc.line);
  buffer_puts(buffer_2, ":");
  buffer_putulong(buffer_2, b->loc.column);
  buffer_putnlflush(buffer_2);
#endif
  for(;;) {
    if(b->p) {
      if(b->chrs[0] == '\n') {
        b->loc.column = 0;
        b->loc.line++;
      } else {
        b->loc.column++;
      }
      if(b->p > 1)
        byte_copy(b->chrs, (b->p - 1), b->chrs + 1);
      return (unsigned int)(unsigned char)b->chrs[--b->p];
    }
    if((ret = charbuf_stubborn_read(b, 1)) > 0)
      b->p = ret;
    else
      break;
  }
  return ret;
}
