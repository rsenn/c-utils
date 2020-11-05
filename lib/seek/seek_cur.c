#define _LARGEFILE_SOURCE 1
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE 1
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

seek_pos
seek_cur(fd_t fd) {
  return lseek(fd, (off_t)0, SEEK_CUR);
}
