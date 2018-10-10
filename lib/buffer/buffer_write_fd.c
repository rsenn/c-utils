#include "../windoze.h"

#define _LARGEFILE64_SOURCE
#include "../buffer.h"
#include "../io_internal.h"
#include <stdlib.h>
#include <sys/stat.h>

int
buffer_write_fd(buffer* b, fd_t fd) {
  b->fd = fd;

  b->p = b->n = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = malloc(BUFFER_OUTSIZE);

  if(b->x == NULL) return -1;
  b->op = (void*)write;
  b->deinit = buffer_free;
  return 0;
}
