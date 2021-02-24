#include "../charbuf_internal.h"

ssize_t
charbuf_get(charbuf* b) {
  ssize_t ret;

  if((ret = charbuf_peek(b)) > 0)
    ret = charbuf_skip(b);
  /*#ifdef DEBUG_CHARBUF
    charbuf_debug = 1;
  #endif*/
  return ret;
}