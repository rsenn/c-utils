#include "../windoze.h"
#if WINDOWS_NATIVE
#include "../io_internal.h"
#else
#include <unistd.h>
#endif

#include "../buffer.h"

static ssize_t
b0read(fd_t fd, char* buf, size_t len) {
  if(buffer_flush(buffer_1small) < 0) return -1;
  return read(fd, buf, len);
}

char buffer_0_space[128];
static buffer it = BUFFER_INIT_READ(b0read, 0, buffer_0_space, sizeof buffer_0_space);
buffer* buffer_0small = &it;
