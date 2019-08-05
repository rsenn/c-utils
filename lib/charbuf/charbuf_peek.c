#include "../charbuf.h"

int
charbuf_peek(charbuf* b) {
  if(!b->p) {
    int ret;
    if((ret = b->op(b->fd, &b->ch, 1, b) <= 0)) return ret;
    b->p = 1;
  }
  return (int)(unsigned int)(unsigned char)b->ch;
}

