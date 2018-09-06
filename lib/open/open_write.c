#define _FILE_OFFSET_BITS 64
#include "../open.h"
#include <fcntl.h>

#ifndef O_NDELAY
#define O_NDELAY 0
#endif

int
open_write(const char* filename) {
  return open(filename, O_WRONLY | O_CREAT | O_NDELAY, 0644);
}
