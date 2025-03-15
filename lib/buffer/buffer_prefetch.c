#include "../buffer.h"
#include "../byte.h"
#include <assert.h>

ssize_t buffer_dummyreadmmap(fd_type, char*, unsigned long int);
ssize_t buffer_stubborn_read(buffer_op_proto*, fd_type, const void*, size_t, void*);

int
buffer_prefetch(buffer* b, size_t n) {
  ssize_t need = n - buffer_LEN(b);

  if(b->p > 0) {
    if(need > buffer_HEADROOM(b))
      if((buffer_op_proto*)b->op == (buffer_op_proto*)(void*)&buffer_dummyreadmmap || b->deinit == (void*)&buffer_munmap)
        goto end;

    if(buffer_LEN(b) == 0 || need > buffer_HEADROOM(b))
      buffer_MOVE(b);
  }

  assert(need <= buffer_HEADROOM(b));

  while(need > 0)
  /*while(buffer_LEN(b) < n) while(b->p + n > b->n)*/ {
    ssize_t r;

    if((r = buffer_stubborn_read(b->op, b->fd, buffer_END(b), buffer_HEADROOM(b), b)) < 0)
      return -1;

    if(!r)
      break;

    buffer_SEEK(b, r);
    need -= r;
  }

end:
  return buffer_LEN(b);
}
