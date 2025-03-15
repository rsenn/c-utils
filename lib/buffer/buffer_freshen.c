#include "../buffer.h"
#include "../byte.h"
#include <assert.h>

ssize_t buffer_dummyread(int, void*, size_t);
ssize_t buffer_dummyreadmmap(int, void*, size_t, void*);
ssize_t buffer_stubborn_read(ssize_t (*)(), int, void*, size_t, void*);

int
buffer_freshen(buffer* b) {
  assert((void*)b->op != (void*)&buffer_dummyread);
  assert((void*)b->op != (void*)&buffer_dummyreadmmap);

  if(b->p > 0)
    buffer_MOVE(b);

  if(b->n < b->a) {
    size_t n = b->a - b->n;
    ssize_t w;

    if((w = buffer_stubborn_read(b->op, b->fd, b->x + b->n, n, b)) < 0)
      return -1;

    b->n += w;
  }

  return b->n - b->p;
}
