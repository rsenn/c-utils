#include "../charbuf.h"

int
charbuf_peekc(charbuf* b, char* ch) {
  int ret;
  if(b->p) {
    *ch = b->ch;
    return 1;
  }
  if((ret = b->op(b->fd, &b->ch, 1, b) <= 0)) {
    if(ret == 0)
      b->eof = 1;
    else if(ret < 0)
      b->err = 1;
  } else {
    b->p = 1;
    *ch = b->ch;
  }

  return ret;
}
