#include "../charbuf_internal.h"

ssize_t
charbuf_getc(charbuf* b, unsigned char* ch) {
  ssize_t ret;
  if((ret = charbuf_peekc(b, ch)) > 0)
    charbuf_skip(b);
  return ret;
}