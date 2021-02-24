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
    buffer_putlong(buffer_2, n);
    buffer_puts(buffer_2, ")");
    charbuf_dump(b, buffer_2);
    buffer_puts(buffer_2, charbuf_colors ? "  " CHARBUF_GRAY "ret" CHARBUF_BLACK CHARBUF_EQ CHARBUF_CYAN : "  ret=");
    for(i = 0; i < n; i++) {
      if(i > 0)
        buffer_putspace(buffer_2);
      charbuf_dumpchar(b->chrs[i], buffer_2);
    }
    buffer_putxlonglong(buffer_2, (uintptr_t)ret);
    if(charbuf_colors)
      buffer_puts(buffer_2, CHARBUF_NC);
    buffer_putnlflush(buffer_2);
  }
#endif

  return ret;
}