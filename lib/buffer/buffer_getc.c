#include "../buffer.h"
#include "../byte.h"

ssize_t
buffer_getc(buffer* b, char* x) {
  if(buffer_EMPTY(b)) {
    ssize_t bytes;

    if((bytes = buffer_feed(b)) <= 0)
      return bytes;
  }

  *x = *buffer_PEEK(b);
  buffer_SKIP(b, 1);

  return 1;
}
