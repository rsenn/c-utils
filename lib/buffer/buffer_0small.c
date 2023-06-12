#include "../windoze.h"
#if WINDOWS_NATIVE
#include "../io_internal.h"
#else
#include <unistd.h>
#endif

#include "../buffer.h"

static ssize_t
buffer_0small_read(fd_t fd, char* buf, size_t len) {
  if(buffer_flush(buffer_1small) < 0)
    return -1;
  return read(fd, buf, len);
}

char buffer_0small_space[128];
static buffer buffer_0small_it = BUFFER_INIT_READ(buffer_0small_read, 0, buffer_0small_space, sizeof buffer_0small_space);
buffer* buffer_0small = &buffer_0small_it;
