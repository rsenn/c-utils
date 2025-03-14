#include "../buffer.h"
#include "../byte.h"
#include <assert.h>

ssize_t buffer_dummyread(int, void*, size_t);
ssize_t buffer_dummyreadmmap(int, void*, size_t, void*);
ssize_t buffer_stubborn_read(ssize_t (*)(), int, void*, size_t, void*);

int
buffer_freshen(buffer* b) {
  size_t bytes;
  assert((void*)b->op != (void*)&buffer_dummyread);
  assert((void*)b->op != (void*)&buffer_dummyreadmmap);

  bytes = b->n - b->p;

  if(b->p > 0 && bytes) {
    byte_copy(b->x, b->n - b->p, &b->x[b->p]);
    b->n -= b->p;
    b->p = 0;
  }

  if(b->p == b->n) {
    b->p = 0;
    b->n = 0;
  }

  if(b->n < b->a) {
    ssize_t w;

    if((w = buffer_stubborn_read(b->op, b->fd, &b->x[b->n], b->a - b->n, b)) > 0)
      b->n += w;

    if(w == -1)
      return -1;
  }
  return b->n - b->p;
}
