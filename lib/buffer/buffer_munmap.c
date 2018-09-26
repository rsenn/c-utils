#include "../windoze.h"

#include "../buffer.h"
#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <sys/mman.h>
#include <fcntl.h>
#endif

void
buffer_munmap(void* buf) {
  buffer* b = (buffer*)buf;
#if WINDOWS
  UnmapViewOfFile(b->x);
#else
  if(b->fd != -1)  {
    int flags;
    if((flags = fcntl(b->fd, F_GETFL)) != -1)
      if(flags & (O_WRONLY|O_RDWR))
        msync(b->x, b->a, MS_SYNC);
  }
  munmap(b->x, b->a);
#endif
  b->x = NULL;
  b->a = 0;
  b->deinit = NULL;
}
