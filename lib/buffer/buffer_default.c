#include "../buffer.h"

/* set sane defaults + operation */
void
buffer_default(buffer* b, ssize_t (*op)(intptr_t fd, void* buf, size_t len, void* arg)) {
  b->fd = -1;
  b->p = 0;
  b->n = 0;
  b->op = op;
}
