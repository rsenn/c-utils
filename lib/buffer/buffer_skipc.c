#include "../buffer.h"

int
buffer_skipc(buffer* b) {
  if(b->p < b->n) {
    if(++b->p == b->n) {
      int r;
      if((r = buffer_feed(b)) <= 0)
        return r;
    }
    return 1;
  }
  return 0;
}
