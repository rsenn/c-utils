#include "../charbuf_internal.h"

ssize_t
charbuf_peek(charbuf* b) {
  ssize_t ret;
  uint8 ch;

  if((ret = charbuf_peekc(b, &ch)) > 0)
    ret = (unsigned int)(unsigned char)b->chrs[0];

  return ret;
}