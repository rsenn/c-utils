#include <unistd.h>
#include <fcntl.h>
#include "open.h"

#ifndef O_NDELAY
#define O_NDELAY 0
#endif

int open_read(const char *filename) {
  return open(filename,O_RDONLY);
}
