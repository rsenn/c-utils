#include "../buffer.h"
#include <errno.h>

ssize_t
buffer_stubborn_read(buffer_op_fn* op, intptr_t fd, const void* buf, size_t len, void* cookie) {
  ssize_t w;
  for(;;) {
    if((w = op(fd, (void*)buf, len, cookie)) < 0)
      if(errno == EINTR) continue;
    break;
  }
  return w;
}
