#include "../buffer.h"
#include "../byte.h"

int
buffer_putalign(buffer* b, const char* x, size_t n) {
  size_t tmp;

  while(n > (tmp = buffer_SPACE(b))) {
    byte_copy(buffer_PEEK(b), tmp, x);
    buffer_SKIP(b, tmp);
    x += tmp;
    n -= tmp;

    if(buffer_flush(b) < 0)
      return -1;
  }

  byte_copy(buffer_PEEK(b), n, x);
  buffer_SKIP(b, n);

  return 0;
}
