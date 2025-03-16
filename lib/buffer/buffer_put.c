#include "../buffer.h"
#include "../byte.h"
#include <string.h>

int buffer_stubborn(buffer_op_proto*, fd_type, const char*, size_t, void*);

#ifdef __dietlibc__
#undef __unlikely
#endif

#ifndef __unlikely
#define __unlikely(x) (x)
#endif

int
buffer_put(buffer* b, const char* x, size_t n) {
  if(__unlikely(b->p + n > b->a)) { /* doesn't fit */
    if(buffer_flush(b) == -1)
      return -1;

    if(n > b->a) {
      if(buffer_stubborn(b->op, b->fd, x, n, b) < 0)
        return -1;

      return 0;
    }
  }

  byte_copy(b->x + b->p, n, x);
  b->p += n;

  return 0;
}
