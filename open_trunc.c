#define _FILE_OFFSET_BITS 64
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // defined HAVE_UNISTD_H
#include <fcntl.h>
#include "open.h"

#ifndef O_NDELAY
#define O_NDELAY 0
#endif

int open_trunc(const char *filename) {
  return open(filename,O_WRONLY|O_NDELAY|O_TRUNC|O_CREAT,0644);
}
