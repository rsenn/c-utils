#include "../charbuf.h"

int
charbuf_skipc(charbuf* b, char* ch) {
  int ret = charbuf_skip(b);
  if(ret >= 0)
    ret = charbuf_peekc(b, ch);
  return ret;
}
