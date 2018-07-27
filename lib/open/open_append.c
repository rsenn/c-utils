#define _FILE_OFFSET_BITS 64
#if defined(_WIN32) || defined(_WIN64)
# include <io.h>
#else
# include <unistd.h>
#endif
#include <fcntl.h>
#include "../open.h"

#ifndef O_NDELAY
#define O_NDELAY 0
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

int open_append(const char* filename) {
  return open(filename, O_WRONLY|O_NDELAY|O_APPEND|O_CREAT|O_BINARY, 0600);
}
