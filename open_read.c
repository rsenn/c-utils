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

#ifndef O_LARGEFILE
#define O_LARGEFILE 0
#endif

int open_read(const char *filename) {
  return open(filename,O_RDONLY | O_LARGEFILE);
}
