#include "../charbuf.h"

int
charbuf_getc(charbuf* b, char* ch) {
  int ret;

  if(b->p) {
    b->p = 0;
    *ch = b->ch;
    return 1;
  }

  if(b->eof || b->err)
    return b->eof ? 0 : -1;

  b->ch = '\0';
  if((ret = charbuf_stubborn_read(b) <= 0)) {
    if(ret == 0)
      b->eof = 1;
    else if(ret < 0)
      b->err = 1;
    return ret;
  } else {
    b->p = 1;
    *ch = b->ch;
  }

  return ret;
}