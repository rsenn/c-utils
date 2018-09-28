#include "../windoze.h"
#include "../buffer.h"
#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

int
buffer_unixread(int fd, char* buf, unsigned int len) {
  return read(fd, buf, len);
}
