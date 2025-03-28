#include "../buffer.h"

ssize_t buffer_stubborn_read(buffer_op_proto*, int, const char*, size_t, void*);

ssize_t
buffer_feed(buffer* b) {
  if(b->p == b->n) {
    ssize_t w;

    if((w = b->op ? buffer_stubborn_read(b->op, b->fd, b->x, b->a, b) : 0) < 0)
      return -1;

    b->n = (size_t)w;
    b->p = 0;
  }

  return b->n - b->p;
}
