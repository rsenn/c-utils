#include "../charbuf_internal.h"
#include "../str.h"

/* Gives the charbuf at most len of the bytes that are left in the string, and
 * stops at its terminator. It looks at no more bytes than it hands out: the
 * length of the whole rest of the string was measured here for every byte
 * that a parser asked for, which made reading a string quadratic. */
static ssize_t
charbuf_readstr(int fd, void* buf, size_t len, void* b) {
  const char* src = ((charbuf*)b)->ptr;
  size_t n = 0;

  (void)fd;

  while(n < len && src[n])
    n++;

  if(n) {
    byte_copy(buf, n, src);
    ((charbuf*)b)->ptr = (void*)(src + n);
  }

  return (ssize_t)n;
}

void
charbuf_froms(charbuf* b, char* s, unsigned lookahead) {
  charbuf_init(b, charbuf_readstr, -1, lookahead);
  b->ptr = (void*)s;
}
