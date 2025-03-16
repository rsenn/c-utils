#include "../buffer.h"
#include "../alloc.h"
#include "../byte.h"

ssize_t buffer_dummyreadbuf(fd_type, void*, size_t, buffer*);

int
buffer_copybuf(buffer* b, const char* x, size_t n) {
  if(!(b->x = (char*)alloc(n)))
    return -1;

  b->p = 0;
  b->n = n;
  b->a = n;
  b->fd = 0;
  b->op = &buffer_dummyreadbuf;
  b->deinit = &buffer_free;

  byte_copy(b->x, n, x);
  return 0;
}
