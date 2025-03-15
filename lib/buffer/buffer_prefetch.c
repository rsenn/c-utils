#include "../buffer.h"
#include "../byte.h"

ssize_t buffer_dummyreadmmap(fd_type, char* buf, unsigned long int len);
ssize_t buffer_stubborn_read(buffer_op_proto*, fd_type fd, const void* buf, size_t len, void* ptr);

int
buffer_prefetch(buffer* b, size_t len) {
  size_t m = len - buffer_LEN(b);

  if(m > buffer_HEADROOM(b)) {
    if((buffer_op_proto*)b->op == (buffer_op_proto*)(void*)&buffer_dummyreadmmap || b->deinit == (void (*)()) & buffer_munmap)
      return buffer_LEN(b);

    if(b->p > 0)
      buffer_MOVE(b);
  }

  if(m > buffer_HEADROOM(b))
    if(!(m = buffer_HEADROOM(b)))
      return -1;

  while(b->p + len > b->n) {
    int w;

    if((w = buffer_stubborn_read(b->op, b->fd, buffer_END(b), buffer_HEADROOM(b), b)) < 0)
      return -1;

    if(!w)
      break;

    buffer_SEEK(b, w);
  }

  return buffer_LEN(b);
}
