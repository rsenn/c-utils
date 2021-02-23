#include "../charbuf.h"

ssize_t
charbuf_stubborn_read(
    read_fn* op, fd_t fd, const void* buf, size_t len, void* ptr) {
  ssize_t w;
  for(;;) {
    if((w = (*op)(fd, (void*)buf, len, ptr)) < 0)
      if(errno == EINTR)
        continue;
    break;
  }
  return w;
}
