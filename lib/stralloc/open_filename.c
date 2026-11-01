#include "../windoze.h"
#include "../stralloc.h"
#include "../typedefs.h"
#include "../mmap.h"

#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <limits.h>
#include "../str.h"
#include "../fmt.h"
#include <fcntl.h>

/* stores the path of the file behind fd in sa; 1 on success, 0 if it is not known */
int
open_filename(fd_type fd, stralloc* sa) {
#ifdef F_GETPATH
  if(!stralloc_ready(sa, PATH_MAX + 1))
    return 0;
  if(fcntl(fd, F_GETPATH, sa->s) != -1) {
    sa->len = str_len(sa->s);
    return 1;
  }
  return 0;
#elif !WINDOWS_NATIVE
  char link[64], path[PATH_MAX + 1];
  ssize_t n;

  link[0] = '\0';
  str_copy(link, "/proc/self/fd/");
  link[14 + fmt_ulong(link + 14, (unsigned long)fd)] = '\0';
  if((n = readlink(link, path, PATH_MAX)) > 0) {
    stralloc_copyb(sa, path, (size_t)n);
    return 1;
  }
  /* no /proc: look the mapping up in the maps of the process */
  {
    size_t len;
    char* x;
    int ret = 0;

    if((x = mmap_read_fd(fd, &len))) {
      ret = mmap_filename(x, sa);
      mmap_unmap(x, len);
    }
    return ret;
  }
#else
  size_t len;
  char* x;
  int ret = 0;

  if((x = mmap_read_fd(fd, &len))) {
    ret = mmap_filename(x, sa);
    mmap_unmap(x, len);
  }
  return ret;
#endif
}
