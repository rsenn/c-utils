#include "buffer.h"
#include <unistd.h>

int
buffer_unixwrite(int fd, const char* buf, unsigned int len) {
  return write(fd, buf, len);
}
