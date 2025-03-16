#include "../buffer.h"

ssize_t
buffer_dummyread(fd_type fd, void* buf, size_t len, buffer* b) {
  return 0;
}
