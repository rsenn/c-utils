#include "../buffer.h"
#include "../byte.h"

int
buffer_putalign(buffer* b, const char* buf, size_t len) {
  size_t tmp;

  while(len > (tmp = buffer_SPACE(b))) {
    byte_copy(buffer_PEEK(b), tmp, buf);
    b->p += tmp;
    buf += tmp;
    len -= tmp;

    if(buffer_flush(b) < 0)
      return -1;
  }

  byte_copy(buffer_PEEK(b), len, buf);
  b->p += len;

  return 0;
}
