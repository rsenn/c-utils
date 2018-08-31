

#include "../buffer.h"
#include "../mmap.h"

ssize_t
buffer_dummyreadmmap() {
  return 0;
}
extern void buffer_munmap(void* buf);

int
buffer_mmapprivate(buffer* b, const char* filename) {
  if(!(b->x = mmap_private(filename, &b->n))) return -1;
  b->p = 0;
  b->a = b->n;
  b->fd = -1;
  b->op = buffer_dummyreadmmap;
  b->deinit = buffer_munmap;
  return 0;
}
