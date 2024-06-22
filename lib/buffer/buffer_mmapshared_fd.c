#include "../windoze.h"
#include "../buffer.h"
#include "../mmap.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

extern ssize_t buffer_dummyreadmmap(fd_type, void*, size_t, void*);
extern void buffer_munmap(void* buf);

int
buffer_mmapshared_fd(buffer* b, fd_type fd) {
  b->fd = dup(fd);
  if(!(b->x = mmap_shared_fd(fd, &b->n)))
    return -1;
  b->p = 0;
  b->a = b->n;
  b->op = (buffer_op_proto*)&buffer_dummyreadmmap;
  b->deinit = (void (*)()) & buffer_munmap;
  return 0;
}
