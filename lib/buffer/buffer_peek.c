#include "../buffer.h"

char*
buffer_peek(buffer* b) {
  if(buffer_prefetch(b, 1) <= 0)
    return NULL;

  return buffer_PEEK(b);
}
