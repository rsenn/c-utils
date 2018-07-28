#include "../charbuf.h"

int
charbuf_skip(charbuf* b) {
  int ret;

  if(b->p)
    b->p = 0;
  else if((ret = b->op(b->fd, &b->ch, 1) <= 0))
    return ret;

  return 1;
}
