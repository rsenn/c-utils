#include "../stralloc.h"
#include "../open.h"
#include "../uint64.h"
#include <errno.h>

#if WINDOWS_NATIVE
#ifdef _MSC_VER
#define _CRT_INTERNAL_NONSTDC_NAMES 1
#endif
#include <io.h>
#if !defined(__LCC__) && !defined(__MINGW32__)
#define read _read
#define write _write
#define open _open
#define close _close
#endif
#else
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

    if((r = read(fd, sa->s + sa->len, bufsize)) == -1)

      if(errno == EINTR)
        continue;

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
