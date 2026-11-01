#include "../windoze.h"
#include "../buffer.h"
#include "../mmap.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

ssize_t buffer_dummyreadmmap(fd_type, void*, size_t, buffer*);
void buffer_munmap(buffer*);

int
buffer_mmapshared_fd(buffer* b, fd_type fd) {
  /* like buffer_mmapread_fd: on success the buffer owns fd (buffer_close closes it) */
  if(!(b->x = mmap_shared_fd(fd, &b->n)))
  {
    /* a buffer that holds nothing, so that a caller who does not look at the result reads an empty file */
    b->x = NULL;
    b->p = b->n = b->a = 0;
    b->fd = -1;
    b->op = &buffer_dummyreadmmap;
    b->deinit = 0;
    b->cookie = 0;
    return -1;
  }

  b->p = 0;
  b->a = b->n;
  b->fd = fd;
  b->op = &buffer_dummyreadmmap;
  b->deinit = &buffer_munmap;
  b->cookie = 0;
  return 0;
}
