#include "../charbuf.h"

ssize_t
charbuf_skip_until(charbuf* b, int c) {
  ssize_t ret;
  size_t n = 0;

  for(;;) {
    if((ret = charbuf_peek(b)) <= 0)
      return ret;
    if(ret == (unsigned int)(unsigned char)c)
      return n;
    if((ret = charbuf_skip(b)) <= 0)
      return ret;
    ++n;
  }
  return ret;
}
