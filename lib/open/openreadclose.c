#include "../windoze.h"
#include "../stralloc.h"
#include "../open.h"
#include "../uint64.h"
#include <errno.h>

#if !WINDOWS_NATIVE
#include <unistd.h>
#endif

int
openreadclose(const char* fn, stralloc* sa, size_t bufsize) {
  int64 fd;
  fd = open_read(fn);
  if(fd == -1) {
    if(errno == ENOENT) return 0;
    return -1;
  }
  if(readclose(fd, sa, bufsize) == -1) return -1;
  return 1;
}
