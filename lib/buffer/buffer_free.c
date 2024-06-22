#include "../alloc.h"
#include "../buffer.h"
#include <stdlib.h>

void
buffer_free(void* buf) {
  buffer* b = (buffer*)buf;

  if(b->x) {
    alloc_free(b->x);
    b->x = 0;
  }
}
