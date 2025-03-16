#include "../windoze.h"

#define _LARGEFILE64_SOURCE
#include "../buffer.h"
#include <stdlib.h>
#include <sys/stat.h>

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#include "../alloc.h"

int
buffer_read_fd(buffer* b, fd_type fd) {
  b->fd = fd;
  b->p = b->n = 0;
  b->a = BUFFER_INSIZE;
  b->x = (char*)alloc(BUFFER_INSIZE);

  if(b->x == NULL)
    return -1;

  b->op = (buffer_op_proto*)(void*)read;
  b->deinit = &buffer_free;
  return 0;
}
