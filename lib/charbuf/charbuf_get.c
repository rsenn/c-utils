#include "../charbuf.h"

ssize_t
charbuf_get(charbuf* b) {
  ssize_t ret;
  if((ret = charbuf_peek(b)) > 0)
    charbuf_skip(b);
  return ret;
}