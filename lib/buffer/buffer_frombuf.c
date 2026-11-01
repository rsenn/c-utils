#include "../buffer.h"

void
buffer_frombuf(buffer* b, const char* x, size_t l) {
  b->x = (char*)x;
  b->p = 0;
  b->n = l;
  b->a = l;
  b->fd = 0;
  b->op = &buffer_dummyreadbuf;
  b->deinit = 0;
  b->cookie = 0;
}
