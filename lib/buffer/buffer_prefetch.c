#include "../buffer.h"
#include "../byte.h"

ssize_t buffer_dummyreadmmap(fd_type, char* buf, unsigned long int len);
ssize_t buffer_stubborn_read(
    buffer_op_proto*, fd_type fd, const void* buf, size_t len, void* ptr);

int
buffer_prefetch(buffer* b, size_t len) {
  size_t n = buffer_LEN(b);
  size_t m = len - n;

  if(buffer_LEN(b) && m > buffer_AVAIL(b)) {
    if((buffer_op_proto*)b->op ==
           (buffer_op_proto*)(void*)&buffer_dummyreadmmap ||
       b->deinit == (void (*)()) & buffer_munmap)
      return buffer_LEN(b);
    byte_copy(b->x, n, buffer_PEEK(b));
    b->n -= b->p;
    b->p = 0;
  }
  if(m > buffer_AVAIL(b))
    m = buffer_AVAIL(b);
  if(m == 0)
    return -1;
  while(b->n < b->p + len) {
    int w;
    if((w = buffer_stubborn_read(
            b->op, b->fd, buffer_END(b), b->a - b->n, b)) < 0)
      return -1;
    b->n += w;
    if(!w)
      break;
  }
  return b->n - b->p;
}
