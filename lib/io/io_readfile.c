#define _FILE_OFFSET_BITS 64
#if defined(_WIN32) || defined(_WIN64)
#else
#endif
#include "../io_internal.h"
#include <fcntl.h>

int
io_readfile(fd_t* d, const char* s) {
  long fd = open(s, O_RDONLY);
  if(fd != -1) {
    *d = fd;
    return 1;
  }
  return 0;
}
