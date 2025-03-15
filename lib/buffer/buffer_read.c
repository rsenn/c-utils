#include "../windoze.h"
#include "../buffer.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

int
buffer_unixread(fd_type fd, char* x, size_t n) {
  return read(fd, x, n);
}
