#include "../windoze.h"
#include "../buffer.h"
#include "../io_internal.h"

#if !WINDOWS_NATIVE
#include <unistd.h>
#endif

int
buffer_unixwrite(int fd, const char* x, unsigned int n) {
  return write(fd, x, n);
}
