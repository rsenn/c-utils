#include "../charbuf.h"

int
charbuf_get(charbuf* b) {
  int ret;

  if(b->p) {
    b->p = 0;
  } else {
    if((ret = b->op(b->fd, &b->ch, 1, b) <= 0)) return ret;
  }

  return (int)(unsigned int)(unsigned char)b->ch;
}
