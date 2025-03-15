#include "../buffer.h"

int
buffer_skipc(buffer* b) {
  if(buffer_EMPTY(b)) {
    ssize_t r;

    if((r = buffer_feed(b)) <= 0)
      return r;
  }

  if(!buffer_EMPTY(b)) {
    ++b->p;
    return 1;
  }

  return 0;
}
