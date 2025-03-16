#include "../buffer.h"

extern int buffer_stubborn(buffer_op_proto*, fd_type, const char*, size_t, void*);

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
