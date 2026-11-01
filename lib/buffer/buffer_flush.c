#include "../buffer.h"


int
buffer_flush(buffer* b) {
  size_t pos = b->p;
  ssize_t r;

  if(!(pos = b->p))
    return 0; /* buffer already empty */

  b->p = 0;

  if((r = buffer_stubborn(b->op, b->fd, b->x, pos, b)) > 0)
    r = 0;

  return r;
}
