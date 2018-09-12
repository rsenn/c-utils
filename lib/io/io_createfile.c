#include "../windoze.h"
#define _FILE_OFFSET_BITS 64
#if WINDOWS
#else
#endif
#include "../io_internal.h"
#include <fcntl.h>

int
io_createfile(fd_t* d, const char* s) {
  long fd = open(s, O_WRONLY | O_CREAT | O_TRUNC, 0600);
  if(fd != -1) {
    *d = fd;
    return 1;
  }
  return 0;
}
