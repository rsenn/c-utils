#include "../charbuf.h"

int
charbuf_skip(charbuf* b) {
  int ret;

  if(b->p) {
    b->p = 0;
    return 1;
  }

  b->ch = '\0';
  if((ret = b->op(b->fd, &b->ch, 1, b) <= 0)) {
    if(ret == 0)
      b->eof = 1;
    else if(ret < 0)
      b->err = 1;
  } else {
    b->p = 0;
  }

  return ret;
}
