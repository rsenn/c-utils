#include "../charbuf.h"
#include "../buffer.h"
#include <assert.h>

ssize_t
charbuf_peekn(charbuf* b, unsigned int n) {
  ssize_t ret = -1;
  assert(n < b->a);
  while(b->p < n) {
    b->chrs[b->p] = '\0';
    if((ret = charbuf_stubborn_read(b, n - b->p)) <= 0)
      return ret;
    b->p += ret;
  }
  if(b->p)
    ret = b->p;

#ifdef DEBUG_CHARBUF_
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

  return ret;
}