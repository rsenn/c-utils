#include "../buffer.h"
#include "../byte.h"

ssize_t buffer_dummyreadmmap(fd_type, char* buf, unsigned long int len);
ssize_t buffer_stubborn_read(buffer_op_proto*, fd_type fd, const void* buf, size_t len, void* ptr);

int
buffer_prefetch(buffer* b, size_t len) {
  size_t n = buffer_LEN(b);
  size_t m = len - n;

  if(buffer_LEN(b) && m > buffer_HEADROOM(b)) {
    if((buffer_op_proto*)b->op == (buffer_op_proto*)(void*)&buffer_dummyreadmmap || b->deinit == (void (*)()) & buffer_munmap)
      return buffer_LEN(b);

    buffer_MOVE(b);
  }

  if(m > buffer_HEADROOM(b))
    m = buffer_HEADROOM(b);

  if(m == 0)
    return -1;

  while(buffer_LEN(b) < len) /*{while(b->p + len > b->n)*/ {
    int w;

    if((w = buffer_stubborn_read(b->op, b->fd, buffer_END(b), b->a - b->n, b)) < 0)
      return -1;

    buffer_SEEK(b, w);

    if(!w)
      break;
  }

  return buffer_LEN(b);
}
