#include "../buffer.h"
#include <unistd.h>

int
buffer_unixread(int fd, char* buf, unsigned int len) {
  return read(fd, buf, len);
}
