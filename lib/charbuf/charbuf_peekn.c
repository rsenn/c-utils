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
      b->p += r;
      continue;
    }
    break;
  }

#ifdef DEBUG_CHARBUF
  if(charbuf_debug) {
    unsigned int i;
    buffer_puts(buffer_2, "charbuf_peekn (");
    charbuf_dumplabel("n", buffer_2);
    buffer_putlong(buffer_2, n);
    buffer_puts(buffer_2, ")");
    charbuf_dump(b, buffer_2);
    charbuf_dumplabel(CHARBUF_SEP "ret", buffer_2);

    charbuf_dumpchars(ret, n, buffer_2, 1);

    if(charbuf_colors)
      buffer_puts(buffer_2, CHARBUF_NC);
    buffer_putnlflush(buffer_2);
  }
#endif

  return ret;
}