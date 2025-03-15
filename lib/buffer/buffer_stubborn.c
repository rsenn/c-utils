#include "../windoze.h"
#include "../buffer.h"
#include <errno.h>

#if !WINDOWS_NATIVE
#include <unistd.h>
#endif

#ifndef EINTR
#define EINTR 4
#endif

int
buffer_stubborn(buffer_op_proto* op, fd_type fd, const char* x, size_t n, void* b) {
  ssize_t w;
  errno = 0;

  while(n) {
    if((w = ((buffer_op_proto*)op)(fd, (void*)x, n, b)) <= 0) {
      if(errno == EINTR)
        continue;

      return -1;
    }

    x += w;
    n -= (size_t)w;
  }

  return 0;
}
