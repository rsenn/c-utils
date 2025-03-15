#include "../buffer.h"

ssize_t buffer_stubborn_read(buffer_op_proto*, int, const char*, size_t, void*);

ssize_t
buffer_feed(buffer* b) {
  if(buffer_EMPTY(b)) {
    ssize_t w;

    if((w = b->op ? buffer_stubborn_read(b->op, b->fd, buffer_BEGIN(b), b->a, b) : 0) < 0)
      return -1;

    b->n = (size_t)w;
    b->p = 0;
  }

  return buffer_LEN(b);
}
