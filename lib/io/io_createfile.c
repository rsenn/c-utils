#define _FILE_OFFSET_BITS 64
#if defined(_WIN32) || defined(_WIN64)
#else
#endif
#include <fcntl.h>
#include "../io_internal.h"

int io_createfile(fd_t* d, const char* s) {
  long fd = open(s, O_WRONLY | O_CREAT | O_TRUNC, 0600);
  if(fd != -1) {
    *d = fd;
    return 1;
  }
  return 0;
}
