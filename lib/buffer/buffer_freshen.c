#include "../buffer.h"
#include "../byte.h"
#include <assert.h>

ssize_t buffer_dummyread(fd_type, void*, size_t, buffer*);
ssize_t buffer_dummyreadmmap(fd_type, void*, size_t, buffer*);
ssize_t buffer_stubborn_read(buffer_op_proto*, fd_type, const void*, size_t, void*);

ssize_t
buffer_freshen(buffer* b) {
  assert((void*)b->op != (void*)&buffer_dummyread);
  assert((void*)b->op != (void*)&buffer_dummyreadmmap);

  if(b->p > 0)
    buffer_MOVE(b);

  if(b->a > b->n) {
    ssize_t w;

    if((w = buffer_stubborn_read(b->op, b->fd, b->x + b->n, b->a - b->n, b)) < 0)
      return -1;

    b->n += w;
  }

  return b->n - b->p;
}
