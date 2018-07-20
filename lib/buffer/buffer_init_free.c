#include "../buffer.h"

void
buffer_init_free(buffer* b, ssize_t (*op)(intptr_t fd, void* buf, size_t len, void* arg), intptr_t fd, char* y, size_t ylen) {

  buffer_init(b, op, fd, y, ylen);
  b->deinit = buffer_free;
}
