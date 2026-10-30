#include "../buffer.h"

/* skips the bytes for which pred is true, stops in front of the first one
 * for which it is false, returns how many were skipped (or the error of
 * the read when nothing is left) */
int
buffer_skip_pred(buffer* b, int (*pred)(int)) {
  int bytes = 0;

  for(;;) {
    ssize_t r;

    if(buffer_EMPTY(b))
      if((r = buffer_feed(b)) <= 0)
        return bytes ? bytes : (int)r;

    if(!pred((unsigned char)b->x[b->p]))
      break;

    ++b->p;
    ++bytes;
  }

  return bytes;
}
