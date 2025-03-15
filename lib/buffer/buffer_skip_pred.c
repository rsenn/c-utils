#include "../buffer.h"

int
buffer_skip_pred(buffer* b, int (*pred)(int)) {
  int bytes = 0;

  for(;;) {
    ssize_t r;

    if(buffer_EMPTY(b))
      if((r = buffer_feed(b)) <= 0)
        return r;

    ++bytes;

    if(!pred(b->x[b->p++]))
      break;
  }

  return bytes;
}
