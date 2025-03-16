#include "../buffer.h"
#include "../byte.h"

ssize_t
buffer_get(buffer* b, char* x, size_t n) {
  ssize_t bytes = 0, r;

  if((ssize_t)n < 0)
    n = (ssize_t)(((size_t)-1) >> 1);

  for(ssize_t len = (ssize_t)n; len > 0; len -= r) {
    if((r = buffer_feed(b)) < 0)
      return r;

    if(r == 0)
      break;

    if(r >= len)
      r = len;

    byte_copy(x, (size_t)r, b->x + b->p);
    b->p += (size_t)r;

    x += (size_t)r;
    bytes += r;
  }

  return bytes;
}
