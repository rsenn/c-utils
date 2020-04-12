#include "../buffer.h"
#include "../mmap.h"

extern ssize_t buffer_dummyreadmmap();
extern void buffer_munmap(void* buf);

int
buffer_mmapprivate(buffer* b, const char* filename) {
  if(!(b->x = mmap_private(filename, &b->n)))
    return -1;
  b->p = 0;
  b->a = b->n;
  b->fd = -1;
  b->op = (buffer_op_proto*)&buffer_dummyreadmmap;
  b->deinit = (void(*)())&buffer_munmap;
  return 0;
}
