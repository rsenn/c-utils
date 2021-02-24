#include "../charbuf.h"

ssize_t
charbuf_get(charbuf* b) {
  ssize_t ret;
#ifdef CHARBUF_DEBUG
  charbuf_debug =0;
#endif
  if((ret = charbuf_peek(b)) > 0)
    charbuf_skip(b);
#ifdef CHARBUF_DEBUG
  charbuf_debug =1;
#endif
  return ret;
}