#include "../buffer.h"

int
buffer_skipn(buffer* b, size_t n) {
  if(b->p + n > b->n) {
    if(buffer_prefetch(b, n) < n)
      return 0;
  }

  if(b->p + n <= b->n) {
    b->p += n;
    return n;
  }
  return 0;
}
