#include "../buffer.h"

int
buffer_copy(buffer* out, buffer* in) {
  ssize_t n;
  char* x;

  for(;;) {
    if((n = buffer_feed(in)) < 0)
      return -2;

    if(!n)
      return 0;
    x = buffer_PEEK(in);

    if(buffer_put(out, x, n) == -1)
      return -3;
    buffer_SEEK(in, n);
  }
}
