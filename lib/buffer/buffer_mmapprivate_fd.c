#include "../buffer.h"
#include "../mmap.h"

ssize_t buffer_dummyreadmmap(fd_type, void*, size_t, buffer*);
void buffer_munmap(buffer*);

int
buffer_mmapprivate_fd(buffer* b, fd_type fd) {
  if(!(b->x = mmap_private_fd(fd, &b->n)))
    return -1;

  b->p = 0;
  b->a = b->n;
  b->fd = fd;
  b->op = &buffer_dummyreadmmap;
  b->deinit = &buffer_munmap;
  return 0;
}
