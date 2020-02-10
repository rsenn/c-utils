#include "../charbuf.h"

int
charbuf_peek(charbuf* b) {
  if(b->eof || b->err)
    return -1;

  if(!b->p) {
    int ret;
    b->ch = '\0';
    if((ret = b->op(b->fd, &b->ch, 1, b) <= 0)) {
      if(ret == 0)
        b->eof = 1;
      else if(ret < 0)
        b->err = 1;
    } else {
      b->p = 1;
    }
  }
  return (int)(unsigned int)(unsigned char)b->ch;
}
