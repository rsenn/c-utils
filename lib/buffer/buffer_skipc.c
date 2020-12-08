#include "../buffer.h"

int
buffer_skipc(buffer* b) {
  if(b->p == b->n) {
    ssize_t r;
    if((r = buffer_feed(b)) <= 0)
      return r;
  }

  if(b->p < b->n) {
    ++b->p;
    return 1;
  }
  return 0;
}
