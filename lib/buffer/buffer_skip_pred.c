#include "../buffer.h"

int
buffer_skip_pred(buffer* b, int (*pred)(int)) {
  int n = 0;

  for(;;) {
    ssize_t r;

    if(b->p == b->n)

      if((r = buffer_feed(b)) <= 0)
        return r;
    n++;

    if(!pred(b->x[b->p++]))
      break;
  }
  return n;
}
