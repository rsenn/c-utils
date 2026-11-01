#include "../windoze.h"

#include "../buffer.h"
#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <sys/mman.h>
#endif
#include <fcntl.h>

void
buffer_munmap(buffer* b) {
#if WINDOWS_NATIVE
  UnmapViewOfFile(b->x);
#else
  if(b->fd != -1) {
    int flags;

    if((flags = fcntl(b->fd, F_GETFL)) != -1)
      if(flags & (O_WRONLY | O_RDWR))
        msync(b->x, b->a, MS_SYNC);
  }

  munmap(b->x, b->a);
#endif

  b->x = NULL;
  b->p = 0;
  b->n = 0; /* the buffer reads as empty afterwards instead of pointing at unmapped memory */
  b->a = 0;
  b->deinit = NULL;
}
