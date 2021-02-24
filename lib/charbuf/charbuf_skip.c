#include "../charbuf_internal.h"

ssize_t
charbuf_skip(charbuf* b) {
  ssize_t ret;
  uint8 ch;

  if((ret = charbuf_skipc(b, &ch)) > 0)
    ret = ch;

  return ret;
}