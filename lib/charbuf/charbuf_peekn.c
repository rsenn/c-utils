#include "../charbuf_internal.h"
#include <assert.h>

uint8*
charbuf_peekn(charbuf* b, unsigned int n) {
  ssize_t r;
  uint8* ret = 0;
  assert(n < b->a);

  for(;;) {
    if(b->p == n) {
      ret = b->chrs;
    } else if((r = charbuf_stubborn_read(b, n - b->p)) > 0) {
      b->p = r;
      continue;
    }
    break;
  }

#ifdef DEBUG_CHARBUF
  if(charbuf_debug) {
    buffer_puts(buffer_2, "charbuf_peekn (");
    buffer_putlong(buffer_2, n);
    buffer_puts(buffer_2, ")");
    charbuf_dump(b, buffer_2);
    buffer_puts(buffer_2, charbuf_colors ? ", \x1b[1;30mret\x1b[m=\x1b[1;37m" : ", ret=");
    buffer_putxlonglong(buffer_2, ret);
    if(charbuf_colors)
      buffer_puts(buffer_2, "\x1b[m");
    buffer_putnlflush(buffer_2);
  }
#endif

  return ret;
}