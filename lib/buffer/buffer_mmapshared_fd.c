#include "../buffer.h"
#include "../mmap.h"

extern ssize_t buffer_dummyreadmmap();
extern void buffer_munmap(void* buf);

int
buffer_mmapshared_fd(buffer* b, int fd) {
  b->fd = dup(fd);
  if(!(b->x = mmap_shared_fd(fd, &b->n))) return -1;
  b->p = 0;
  b->a = b->n;
  b->op = buffer_dummyreadmmap;
  b->deinit = buffer_munmap;
  return 0;
}
