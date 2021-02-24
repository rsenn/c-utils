#include "../charbuf.h"
#include "../buffer.h"

#define max(a, b) ((a) >= (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))

ssize_t
charbuf_skipn(charbuf* b, size_t n) {
  ssize_t i = 0, rem, pos;

  pos = max(n, b->p);
  rem = (ssize_t)b->p - n;
  if(rem > 0) {
    byte_copy(b->chrs, rem, &b->chrs[pos]);
    b->p = rem;
    i = rem;
  }

  while(i < n) {
    charbuf_skip(b);
    i++;
  }
  return i;
}
