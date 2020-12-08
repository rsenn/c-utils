#include "../buffer.h"
#include "../open.h"

int
buffer_readfile(buffer* b, const char* fn) {
  fd_t fd;
  if((fd = open_read(fn)) == -1)
    return -1;
  return buffer_read_fd(b, fd);
}
