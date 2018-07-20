#include "../buffer.h"

void
buffer_init(buffer* b, buffer_op_fn* op, intptr_t fd, char* y, size_t ylen) {
  b->op = op;
  b->fd = fd;
  b->x = y;
  b->a = ylen;
  b->p = 0;
  b->n = 0;
  b->cookie = 0;
  b->deinit = 0;
}
