#include "../buffer.h"

int
buffer_peekc(buffer* b, char* c) {
  ssize_t r;

  if((r = buffer_prefetch(b, 1)) > 0) {
    *c = *buffer_PEEK(b);
    return 1;
  }
  return r;
}
