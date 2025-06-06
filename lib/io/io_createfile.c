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
io_createfile(fd_type* d, const char* s) {
  long fd = open(s,
                 O_WRONLY | O_CREAT | O_TRUNC
#if !WINDOWS_NATIVE
                 ,
                 0600
#endif
  );

  if(fd != -1) {
    *d = fd;
    return 1;
  }
  return 0;
}
