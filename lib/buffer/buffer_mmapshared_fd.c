#include "../windoze.h"
#include "../buffer.h"
#include "../mmap.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

extern ssize_t buffer_dummyreadmmap();
extern void buffer_munmap(void* buf);

int
buffer_mmapshared_fd(buffer* b, fd_t fd) {
  b->fd = dup(fd);
  if(!(b->x = mmap_shared_fd(fd, &b->n))) return -1;
  b->p = 0;
  b->a = b->n;
  b->op = buffer_dummyreadmmap;
  b->deinit = buffer_munmap;
  return 0;
}
