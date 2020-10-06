#include "../open.h"
#include <fcntl.h>

#ifndef O_SYNC
#define O_SYNC 0x101000
#endif

#ifndef O_DIRECT
#define O_DIRECT 0x4000
#endif

int
open_rwsync(const char* filename) {
  int fd = open_rw(filename);

  int flags = fcntl(fd, F_GETFL);

  if(fcntl(fd, F_SETFL, flags | O_SYNC | O_DIRECT) == -1 && fcntl(fd, F_SETFL, flags | O_SYNC) == -1 && fcntl(fd, F_SETFL, flags | O_DIRECT) == -1) {
    close(fd);
    return -1;
  }
  return fd;
}
