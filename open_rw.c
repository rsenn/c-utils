#define _FILE_OFFSET_BITS 64
#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>
#include "open.h"

#ifndef O_NDELAY
#define O_NDELAY 0
#endif

int open_rw(const char *filename) {
  return open(filename,O_RDWR|O_CREAT|O_NDELAY,0644);
}
