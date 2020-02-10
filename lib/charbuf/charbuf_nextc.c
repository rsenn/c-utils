#include "../charbuf.h"

int
charbuf_nextc(charbuf* b, char* ch) {
  int ret = charbuf_skip(b);
  if(ret >= 1 && !b->err && !b->eof)
    ret = charbuf_peekc(b, ch);
  return ret;
}
