#include "../buffer.h"

ssize_t
buffer_dummyreadbuf(fd_type fd, void* x, size_t n, struct buffer* b) {
  (void)fd;
  (void)x;
  (void)n;
  (void)b;
  return 0;
}
