#include "../windoze.h"
#include "../open.h"

#include <fcntl.h>
#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#ifndef O_SYNC
#define O_SYNC 0x101000
#endif

#ifndef O_DIRECT
#define O_DIRECT 0x4000
#endif

int
open_rwsync(const char* filename) {
  int fd = open_rw(filename);

#ifdef F_GETFL
  int flags = fcntl(fd, F_GETFL);

  if(fcntl(fd, F_SETFL, flags | O_SYNC | O_DIRECT) == -1 && fcntl(fd, F_SETFL, flags | O_SYNC) == -1 && fcntl(fd, F_SETFL, flags | O_DIRECT) == -1) {
    close(fd);
    return -1;
  }
#endif
  return fd;
}
