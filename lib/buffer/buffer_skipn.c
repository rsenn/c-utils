#include "../buffer.h"

int
buffer_skipn(buffer* b, size_t n) {
  if(b->p + n <= b->n) {
    b->p += n;
    return n;
  }
  return 0;
}

