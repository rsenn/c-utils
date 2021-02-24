#include "../charbuf_internal.h"

ssize_t
charbuf_nextc(charbuf* b, unsigned char* ch) {
  ssize_t ret;
  if((ret = charbuf_skip(b)) > 0)
    ret = charbuf_peekc(b, ch);
  return ret;
}
