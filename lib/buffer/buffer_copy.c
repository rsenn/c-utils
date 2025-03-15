#include "../buffer.h"

int
buffer_copy(buffer* out, buffer* in) {
  for(;;) {
    ssize_t r;
    char* x;

    if((r = buffer_feed(in)) < 0)
      return -2;

    if(!r)
      return 0;

    x = buffer_PEEK(in);

    if(buffer_put(out, x, r) == -1)
      return -3;

    buffer_SKIP(in, r);
  }
}
