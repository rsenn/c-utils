#include "../stralloc.h"
#include "../typedefs.h"

#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <limits.h>
#include <fcntl.h>

int
open_filename(fd_t fd, stralloc* sa) {
  stralloc_ready(sa, MAXPATHLEN + 1);
  if(fcntl(fd, F_GETPATH, sa.s) != -1) {
    sa.len = str_len(sa.s);
    return 1;
  } else {
    size_t n;
    char* x;
    int ret = 0;
    
    if((x = mmap_read_fd(fd, &n)))
      ret = mmap_filename(x, sa);

    mmap_unmap(x, n);
   
    return ret;
  }
}
