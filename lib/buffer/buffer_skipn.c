#include "../buffer.h"

int
buffer_skipn(buffer* b, size_t len) {
  if(b->p + len > b->n) 
    if(buffer_prefetch(b, len) < len)
      return 0;

  if(b->p + len <= b->n) {
    b->p += len;
    return len;
  }

  return 0;
}
