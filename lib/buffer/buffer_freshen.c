#include "../buffer.h"
#include "../byte.h"
#include <assert.h>

ssize_t buffer_dummyread(int, void*, size_t);
ssize_t buffer_dummyreadmmap(int, void*, size_t, void*);
ssize_t buffer_stubborn_read(ssize_t (*)(), int, void*, size_t, void*);

int
buffer_freshen(buffer* b) {
  size_t len;

  assert((void*)b->op != (void*)&buffer_dummyread);
  assert((void*)b->op != (void*)&buffer_dummyreadmmap);

  if(b->p > 0)
    buffer_MOVE(b);

  if((len = buffer_HEADROOM(b)) > 0) {
    ssize_t w;

    if((w = buffer_stubborn_read(b->op, b->fd, buffer_END(b), len, b)) < 0)
      return -1;

    buffer_SEEK(b, w);
  }

  return buffer_LEN(b);
}
