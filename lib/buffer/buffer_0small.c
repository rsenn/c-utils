#if !(defined(_WIN32) || defined(_WIN64))
#include <unistd.h>
#else
#include "../io_internal.h"
#endif
#include "../buffer.h"

static ssize_t
b0read(intptr_t fd, char* buf, size_t len) {
  if(buffer_flush(buffer_1small) < 0) return -1;
  return read(fd, buf, len);
}

char buffer_0_space[128];
static buffer it = BUFFER_INIT_READ(b0read, 0, buffer_0_space, sizeof buffer_0_space);
buffer* buffer_0small = &it;
