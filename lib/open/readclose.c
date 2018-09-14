#include "../open.h"
#include "../windoze.h"
#include "../stralloc.h"
#include "../uint64.h"
#include <errno.h>

#if !WINDOWS_NATIVE
#include <unistd.h>
#endif

ssize_t
readclose_append(int64 fd, stralloc* sa, size_t bufsize) {
  ssize_t r;
  for(;;) {
    if(!stralloc_readyplus(sa, bufsize)) {
      close(fd);
      return -1;
    }
    r = read(fd, sa->s + sa->len, bufsize);
    if(r == -1)
      if(errno == EINTR) continue;
    if(r <= 0) {
      close(fd);
      return r;
    }
    sa->len += (size_t)r;
  }
}

ssize_t
readclose(int64 fd, stralloc* sa, size_t bufsize) {
  if(!stralloc_copys(sa, "")) {
    close(fd);
    return -1;
  }
  return readclose_append(fd, sa, bufsize);
}
