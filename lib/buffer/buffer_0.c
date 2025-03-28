#include "../windoze.h"
#if WINDOWS_NATIVE
#include "../io_internal.h"
#else
#include <unistd.h>
#endif

#include "../buffer.h"

static ssize_t
b0read(fd_type fd, char* x, size_t n) {
  if(buffer_flush(buffer_1) < 0)
    return -1;

  return read(fd, x, n);
}

char buffer_0_space[BUFFER_INSIZE];
static buffer buffer_0_it = BUFFER_INIT_READ(b0read, 0, buffer_0_space, sizeof buffer_0_space);
buffer* buffer_0 = &buffer_0_it;
