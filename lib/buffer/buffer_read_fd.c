#define _LARGEFILE64_SOURCE
#include "../buffer.h"
#include <stdlib.h>
#include <sys/stat.h>

#if defined(_WIN32) || defined(_WIN64)
#include "../windoze.h"
#else
#include <unistd.h>
#endif

int
buffer_read_fd(buffer* b, int fd) {
  struct stat st;
  if(fstat(fd, &st) == -1) return -1;

  if((st.st_mode & S_IFMT) == S_IFREG)
    return buffer_mmapread_fd(b, fd);

  b->fd = fd;

  b->p = b->n = 0;
  b->a = BUFFER_INSIZE;
  b->x = malloc(BUFFER_INSIZE);

  if(b->x == NULL) return -1;
  b->op = read;
  b->deinit = buffer_free;
  return 0;
}
