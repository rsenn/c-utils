#include "../charbuf.h"

int
charbuf_peekc(charbuf* b, char* ch) {
  int ret = b->p;
  if(!b->p) {
    if((ret = b->op(b->fd, &b->ch, 1, b) <= 0)) {
      if(ret == 0)
        b->eof = 1;
      else if(ret < 0)
        b->err = 1;
      return ret;
    }
    ret = b->p = 1;
  }
  if(b->p  && ch)
    *ch = b->ch;

  return ret;
}
