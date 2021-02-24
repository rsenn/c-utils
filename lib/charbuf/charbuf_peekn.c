#include "../charbuf_internal.h"
#include <assert.h>

uint8*
charbuf_peekn(charbuf* b, unsigned int n) {
  ssize_t r;
  uint8* ret = 0;
  assert(n < b->a);
#ifdef DEBUG_CHARBUF
  int debug = charbuf_debug;

  charbuf_debug = 0;
#endif

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
  if(debug) {
    size_t i, pos, len;
    uint8* x;
    buffer_puts(buffer_2, "charbuf_peekn( ");
    charbuf_dumplabel("n", buffer_2);
    buffer_putlong(buffer_2, n);
    buffer_puts(buffer_2, " )");

    charbuf_dump_setcolumn(CHARBUF_COLUMN_1, buffer_2);

    charbuf_dumpchars(ret, n, buffer_2, 1);

    charbuf_dump(b, buffer_2);
    if(charbuf_colors)
      buffer_puts(buffer_2, CHARBUF_NC);
    buffer_putnlflush(buffer_2);
  }
  charbuf_debug = debug;
#endif

  return ret;
}