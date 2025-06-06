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

seek_pos
seek_cur(fd_type fd) {
  return io_seek(fd, (seek_pos)0, SEEK_CUR);
}
