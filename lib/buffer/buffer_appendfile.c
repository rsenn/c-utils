#include "../buffer.h"
#include "../open.h"

int
buffer_appendfile(buffer* b, const char* fn) {
  fd_t fd;
  if((fd = open_append(fn)) == -1)
    return -1;
  return buffer_write_fd(b, fd);
}
