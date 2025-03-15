#include "../buffer.h"

ssize_t
buffer_dummyreadbuf(fd_type fd, void* x, size_t n, void* arg) {
  (void)fd;
  (void)x;
  (void)n;
  return 0;
}

void
buffer_frombuf(buffer* b, const char* x, size_t l) {
  b->x = (char*)x;
  b->p = 0;
  b->n = l;
  b->a = l;
  b->fd = 0;
  b->op = &buffer_dummyreadbuf;
  b->deinit = 0;
}
