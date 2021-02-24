#include "../charbuf.h"

ssize_t
charbuf_next(charbuf* b) {
  ssize_t ret;
  if((ret = charbuf_skip(b)) > 0)
    ret = charbuf_peek(b);
  return ret;
}
