#include "../buffer.h"

void
buffer_init_free(buffer* b, buffer_op_proto* op, fd_type fd, char* y, size_t ylen) {
  buffer_init(b, op, fd, y, ylen);
  b->deinit = &buffer_free;
}
