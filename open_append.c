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

int open_append(const char *filename) {
  return open(filename,O_WRONLY|O_NDELAY|O_APPEND|O_CREAT,0600);
}
