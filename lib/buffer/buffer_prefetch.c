#include "../buffer.h"
#include "../byte.h"

ssize_t buffer_dummyreadmmap(fd_type, void*, size_t, buffer*);
ssize_t buffer_dummyread(fd_type, char*, size_t, buffer*);
ssize_t buffer_stubborn_read(buffer_op_proto*, fd_type, const void*, size_t, void*);

ssize_t
buffer_prefetch(buffer* b, size_t n) {
  if(b->p > 0 && b->p + n > b->a) {
    if(b->op == &buffer_dummyreadmmap || (void*)b->deinit == (void*)&buffer_munmap)
      return b->n - b->p;

    buffer_MOVE(b);
  }

  if(b->p + n > b->a)
    n = b->a - b->p;

  if(n == 0)
    return -1;

  while(b->n < b->p + n) {
    int w;

    if((w = buffer_stubborn_read(b->op, b->fd, &b->x[b->n], b->a - b->n, b)) < 0)
      return -1;

    if(!w)
      break;

    b->n += w;
  }

  return b->n - b->p;
}
