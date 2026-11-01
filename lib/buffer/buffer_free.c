#include "../alloc.h"
#include "../buffer.h"
#include <stdlib.h>

void
buffer_free(buffer* b) {
  if(b->x) {
    alloc_free(b->x);
    b->x = 0;
    b->p = b->n = b->a = 0;
  }
}
