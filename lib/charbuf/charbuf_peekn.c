#include "../charbuf.h"
#include "../buffer.h"
#include <assert.h>

uint8*
charbuf_peekn(charbuf* b, unsigned int n) {
  ssize_t ret;
  assert(n < b->a);

  for(;;) {
    if(b->p == n)
      return b->chrs;
    b->chrs[b->p] = '\0';
    if((ret = charbuf_stubborn_read(b, n - b->p)) <= 0)
      break;
    b->p += ret;
  }

#ifdef DEBUG_CHARBUF
  buffer_puts(buffer_2, "charbuf_peekn (");
  buffer_putlong(buffer_2, n);
  buffer_puts(buffer_2, ") eof=");
  buffer_putulong(buffer_2, b->eof ? 1 : 0);
  buffer_puts(buffer_2, " err=");
  buffer_putulong(buffer_2, b->err ? 1 : 0);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);
#endif

  return 0;
}