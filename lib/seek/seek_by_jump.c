#define _LARGEFILE_SOURCE 1
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE 1
#include "../seek.h"
#include "../io.h"
#include "../errmsg.h"
//#include <fcntl.h>
#include <errno.h>
#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#endif

void
seek_by_jump(fd_t fd, int64 amount) {
  if(amount && io_seek(fd, amount, SEEK_CUR) == (int64)-1) {
    if(errno == ESPIPE)
      seek_by_read(fd, amount);
    else
      diesys(1, "seek failure", 0);
  }
}
