#include "../charbuf.h"

int
charbuf_next(charbuf* b) {
  int ret;
  if((ret = charbuf_skip(b)) > 0)
    ret = charbuf_peek(b);
  return ret;
}
