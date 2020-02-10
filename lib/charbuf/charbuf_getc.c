#include "../charbuf.h"

int
charbuf_getc(charbuf* b, char* ch) {
  int ret = 1;
   if(b->eof || b->err) 
    return b->eof ? 0 : -1;

  if(b->p) {
    b->p = 0;
  }else {
    if((ret = b->op(b->fd, &b->ch, 1, b) <= 0)) {
      if(ret == 0)
        b->eof = 1;
      else if(ret < 0)
        b->err = 1;
    }
    b->p = 1;
  }
  if(b->p && ch)
    *ch = b->ch;
  return ret;
}