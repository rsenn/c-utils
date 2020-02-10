#include "../charbuf.h"

int
charbuf_get(charbuf* b) {
  int ret;

  if(b->p) {
    b->p = 0;
  } else {
    if((ret = b->op(b->fd, &b->ch, 1, b) <= 0)) {
      if(ret == 0)
        b->eof = 1;
      else if(ret == -1)
        b->err = 1;
      return ret;
    }
  }

  return (int)(unsigned int)(unsigned char)b->ch;
}
