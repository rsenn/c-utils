#include "../buffer.h"
#include "../mmap.h"

extern int buffer_dummyreadmmap();

int
buffer_mmapread(buffer* b, const char* filename) {
  if(!(b->x = (char*)mmap_read(filename, &b->n))) return -1;
  b->p = 0;
  b->a = b->n;
  b->fd = -1;
  b->op = (void*)&buffer_dummyreadmmap;
  b->deinit = buffer_munmap;
  return 0;
}
