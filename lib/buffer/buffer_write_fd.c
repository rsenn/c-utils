#define _LARGEFILE64_SOURCE
#include "../windoze.h"
#include "../alloc.h"
#include "../buffer.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>

int
buffer_write_fd(buffer* b, fd_t fd) {
  b->fd = fd;

  b->p = b->n = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = (char*)alloc(BUFFER_OUTSIZE);

  if(b->x == NULL)
    return -1;
  b->op = (buffer_op_proto*)&write;
  b->deinit = (void(*)())&buffer_free;
  return 0;
}
