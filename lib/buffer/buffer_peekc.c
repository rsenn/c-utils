#include "../buffer.h"

int
buffer_peekc(buffer* b, char* c) {
  ssize_t r;

  if(b->p)

  if((r = buffer_prefetch(b, 1)) > 0) {
    *c = b->x[b->p];
    return 1;
  }
 return r;
}
