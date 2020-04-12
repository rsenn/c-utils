#include "../alloc.h"
#include "../buffer.h"
#include <stdlib.h>

void
buffer_free(void* buf) {
  buffer* b = (buffer*)buf;
  alloc_free(b->x);
}
