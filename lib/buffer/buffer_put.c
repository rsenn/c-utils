#include "../buffer.h"
#include "../byte.h"
#include <string.h>

extern int buffer_stubborn(buffer_op_proto* op, fd_type fd, const char* buf, size_t len, void* b);

#ifdef __dietlibc__
#undef __unlikely
#endif

#ifndef __unlikely
#define __unlikely(x) (x)
#endif

int
buffer_put(buffer* b, const char* x, size_t n) {
  if(__unlikely(n > buffer_SPACE(b))) { /* doesn't fit */

    if(buffer_flush(b) == -1)
      return -1;

    if(n > b->a) {
      if(buffer_stubborn(b->op, b->fd, x, n, b) < 0)
        return -1;

      return 0;
    }
  }

  byte_copy(buffer_PEEK(b), n, x);
  buffer_SKIP(b, n);

  return 0;
}
