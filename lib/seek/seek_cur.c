#include "../seek.h"
#include <stdio.h>
#include <errno.h>
#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#endif

#define CUR SEEK_CUR /* sigh */

seek_pos
seek_cur(fd_t fd) {
  return lseek(fd, (int64)0, CUR);
}
