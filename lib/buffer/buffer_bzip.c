#ifdef HAVE_LIBBZ2
#include <bzlib.h>
#define BZ_NO_STDIO 1
#endif

#include "../buffer.h"
#include <stdlib.h>

#if HAVE_LIBBZ2
#ifndef BZ_NO_STDIO
static void
buffer_bzip_close(buffer* b) {
  BZFILE* f;

  if((f = (BZFILE*)b->cookie)) {
    BZ2_bzflush(f);
    BZ2_bzclose(f);
  }
}

static ssize_t
buffer_bunzip_read(fd_type fd, void* x, size_t n, void* b) {
  BZFILE* f = ((buffer*)b)->cookie;
  return BZ2_bzread(f, x, n);
}

int
buffer_bunzip(buffer* b, const char* filename) {
  BZFILE* f;

  if((f = BZ2_bzopen(filename, "rb")) == NULL)
    return -1;
  b->fd = -1;
  b->cookie = f;
  b->n = b->p = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = malloc(b->a);
  b->deinit = &buffer_bzip_close;
  return 0;
}

int
buffer_bunzip_fd(buffer* b, fd_type fd) {
  BZFILE* f;

  if((f = BZ2_bzdopen(fd, "rb")) == NULL)
    return -1;
  b->fd = -1;
  b->cookie = f;
  b->op = &buffer_bunzip_read;
  b->n = b->p = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = malloc(b->a);
  b->deinit = &buffer_bzip_close;
  return 0;
}

static ssize_t
buffer_bzip_write(fd_type fd, void* x, size_t n, void* b) {
  BZFILE* f = ((buffer*)b)->cookie;
  return BZ2_bzwrite(f, x, n);
}

int
buffer_bzip(buffer* b, const char* filename, int level) {
  BZFILE* f;
  char mode[4] = "wb0";
  mode[2] = '0' + (level % 10);

  if((f = BZ2_bzopen(filename, mode)) == NULL)
    return -1;
  b->fd = -1;
  b->cookie = f;
  b->op = &buffer_bzip_write;
  b->n = b->p = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = malloc(b->a);
  b->deinit = &buffer_bzip_close;
  return 0;
}

int
buffer_bzip_fd(buffer* b, fd_type fd, int level) {
  BZFILE* f;
  char mode[4] = "wb0";
  mode[2] = '0' + (level % 10);

  if((f = BZ2_bzdopen(fd, mode)) == NULL)
    return -1;
  b->fd = -1;
  b->cookie = f;
  b->op = &buffer_bzip_write;
  b->n = b->p = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = malloc(b->a);
  b->deinit = &buffer_bzip_close;
  return 0;
}
#endif
#endif
