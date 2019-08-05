#include "../charbuf.h"

int
charbuf_peekc(charbuf* b, char* ch) {
  int ret = charbuf_peek(b);
  if(ret >= 0) {
    *ch = ret;
    ret = 1;
  }
  return ret;
}

