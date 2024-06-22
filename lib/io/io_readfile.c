#include "../io_internal.h"
#define _FILE_OFFSET_BITS 64
#include "../windoze.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#include <fcntl.h>

int
io_readfile(fd_type* d, const char* s) {
  long fd = open(s, O_RDONLY);
  if(fd != -1) {
    *d = fd;
    return 1;
  }
  return 0;
}
