#include "../seek.h"
#include "../errmsg.h"
#include <stdio.h>
#include <errno.h>
#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#endif

void
seek_by_jump(fd_t fd, int64 amount) {
  if(amount && lseek(fd, amount, SEEK_CUR) == (int64)-1) {
    if(errno == ESPIPE)
      seek_by_read(fd, amount);
    else
      diesys(1, "seek failure", 0);
  }
}
