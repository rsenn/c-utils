#define _FILE_OFFSET_BITS 64
#include "../seek.h"
#include "../io.h"
#include <fcntl.h>
#include <errno.h>
#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#endif

#define SET SEEK_SET /* sigh */

int
seek_set(fd_type fd, seek_pos pos) {
  if(io_seek(fd, pos, SET) == -1)
    return -1;

  return 0;
}
