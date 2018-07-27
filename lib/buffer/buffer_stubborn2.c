#include "../buffer.h"
#include <errno.h>

ssize_t
buffer_stubborn_read(buffer_op_proto* op, int fd, const void* buf, size_t len, void* ptr) {
  ssize_t w;
  for(;;) {
    if((w = op(fd, (void*)buf, len, ptr)) < 0)
      if(errno == EINTR) continue;
    break;
  }
  return w;
}
