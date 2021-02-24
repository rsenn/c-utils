#include "../buffer.h"
#include "../charbuf.h"
 
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

      b->offset++;

    } else if((ret = charbuf_stubborn_read(b, 1)) > 0) {
      b->p = ret;
      continue;
    }
    break;
  }

#ifdef DEBUG_CHARBUF
   if(charbuf_debug) {
 buffer_puts(buffer_2, "charbuf_skip ret=");
  if(ret > 0x20 || ret == 0x0a || ret == 0x0d || ret == 9) {
    buffer_putc(buffer_2, '\'');
    if(ret == 0x0a || ret == 0x0d || ret == 0x09)
      buffer_puts(buffer_2, ret == '\n' ? "\\n" : ret == '\r' ? "\\r" : "\\t");
    else
      buffer_putc(buffer_2, ret);
    buffer_putc(buffer_2, '\'');
  } else if(ret > 0 && ret < 0x20) {
    buffer_puts(buffer_2, "x");
    buffer_putxlong0(buffer_2, ret, 2);
  } else {
    buffer_putlong(buffer_2, ret);
  }
  charbuf_dump(b, buffer_2);

}
#endif
  return ret;
}
