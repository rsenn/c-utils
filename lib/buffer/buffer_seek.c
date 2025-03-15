#include "../buffer.h"
#include <assert.h>

void
buffer_seek(buffer* b, size_t len) {
  size_t pos = b->p + len;

  assert(pos >= b->p);
  assert(pos <= b->n);

  b->p = pos;
}
