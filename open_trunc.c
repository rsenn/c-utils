#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>
#include "open.h"

#ifndef O_NDELAY
#define O_NDELAY 0
#endif

int open_trunc(const char *filename) {
  return open(filename,O_WRONLY|O_TRUNC|O_CREAT,0644);
}
