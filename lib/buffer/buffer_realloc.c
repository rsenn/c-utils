#include "../buffer.h"
#include "../alloc.h"

void
buffer_realloc(buffer* b, size_t ylen) {
  alloc_re(&b->x, b->a, ylen);
  b->a = ylen;
  b->deinit = &buffer_free;
}
