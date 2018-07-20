#include "../buffer.h"

int
buffer_peekc(buffer* b, char* c) {
  int r;
  if((r = buffer_prefetch(b, 1)) > 0)   {
    *c = b->x[b->p];
    return 1;
  }
  return r;
}
