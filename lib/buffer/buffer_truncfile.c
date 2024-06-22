#include "../buffer.h"
#include "../open.h"

int
buffer_truncfile(buffer* b, const char* fn) {
  fd_type fd;
  if((fd = open_trunc(fn)) == -1)
    return -1;
  return buffer_write_fd(b, fd);
}
