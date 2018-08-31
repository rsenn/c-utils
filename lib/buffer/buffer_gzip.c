#include "../buffer.h"
#include <stdlib.h>

#ifdef HAVE_ZLIB
#include <zlib.h>

static void
buffer_gz_close(buffer* b) {
  gzFile f;
  if((f = (gzFile)b->cookie)) {
    gzflush(f, Z_FINISH);
    gzclose(f);
  }
}

static ssize_t
buffer_gunzip_read(fd_t fd, void* x, size_t n, void* b) {
  gzFile f = ((buffer*)b)->cookie;
  return gzread(f, x, n);
}

int
buffer_gunzip(buffer* b, const char* filename) {
  gzFile f;
  if((f = gzopen(filename, "rb")) == NULL) return -1;
  b->fd = -1;
  b->cookie = f;
  b->op = &buffer_gunzip_read;
  b->n = b->p = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = malloc(b->a);
  b->deinit = buffer_gz_close;
  return 0;
}

int
buffer_gunzip_fd(buffer* b, fd_t fd) {
  gzFile f;
  if((f = gzdopen(fd, "rb")) == NULL) return -1;
  b->fd = -1;
  b->cookie = f;
  b->op = &buffer_gunzip_read;
  b->n = b->p = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = malloc(b->a);
  b->deinit = buffer_gz_close;
  return 0;
}

static ssize_t
buffer_gzip_write(fd_t fd, void* x, size_t n, void* b) {
  gzFile f = ((buffer*)b)->cookie;
  return gzwrite(f, x, n);
}

int
buffer_gzip(buffer* b, const char* filename, int level) {
  char mode[4] = {'w', 'b', '0' + (level % 10), '\0'};
  gzFile f;
  if((f = gzopen(filename, mode)) == NULL) return -1;
  b->fd = -1;
  b->cookie = f;
  b->op = &buffer_gzip_write;
  b->n = b->p = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = malloc(b->a);
  b->deinit = buffer_gz_close;
  return 0;
}

int
buffer_gzip_fd(buffer* b, fd_t fd, int level) {
  char mode[4] = {'w', 'b', '0' + (level % 10), '\0'};
  gzFile f;
  if((f = gzdopen(fd, mode)) == NULL) return -1;
  b->fd = -1;
  b->cookie = f;
  b->op = &buffer_gzip_write;
  b->n = b->p = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = malloc(b->a);
  b->deinit = buffer_gz_close;
  return 0;
}
#endif