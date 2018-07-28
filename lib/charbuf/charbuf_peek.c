#include "lib/charbuf.h"

int
charbuf_peek(charbuf* b) {
  int ret;

  if(!b->p) {
    if((ret = b->op(b->fd, &b->ch, 1) <= 0))
      return ret;
    b->p = 1;
  }
  return (int)(unsigned int)(unsigned char)b->ch;
}
