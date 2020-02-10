#include "../charbuf.h"

int
charbuf_getc(charbuf* b, char* ch) {
  int ret = 0;
  if(b->p) {
    b->p = 0;
    ret = 1;

  } else {
    if((ret = b->op(b->fd, &b->ch, 1, b) <= 0)) {
      if(ret == 0)
        b->eof = 1;
      else if(ret < 0)
        b->err = 1;
    }
  }
  if(ret > 0 && ch)
    *ch = b->ch;
  return ret;
}