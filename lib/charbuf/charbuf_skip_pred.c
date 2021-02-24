#include "../charbuf.h"

ssize_t
charbuf_skip_pred(charbuf* b, int (*pred)(int)) {
  ssize_t ret;
  size_t n = 0;

  for(;;) {
    if((ret = charbuf_peek(b)) <= 0)
      return ret;
    if(!pred((unsigned int)(unsigned char)ret))
      return n;
    if((ret = charbuf_skip(b)) <= 0)
      return ret;
    ++n;
  }
  return n;
}
