#include "../buffer.h"
#include "../alloc.h"
#include "../byte.h"

extern ssize_t
buffer_dummyreadbuf(fd_type fd, void* buf, size_t len, void* arg);

int
buffer_copybuf(buffer* b, const char* x, size_t l) {
  if(!(b->x = (char*)alloc(l)))
    return -1;

  b->p = 0;
  b->n = l;
  b->a = l;
  b->fd = 0;
  b->op = &buffer_dummyreadbuf;
  b->deinit = &buffer_free;

  byte_copy(b->x, l, x);
  return 0;
}
