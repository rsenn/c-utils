#include "../seek.h"
#include <stdio.h>
#include <errno.h>
#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#endif

#define SET SEEK_SET /* sigh */

int
seek_set(fd_t fd, seek_pos pos) {
  if(lseek(fd, (int64)pos, SET) == -1)
    return -1;
  return 0;
}
