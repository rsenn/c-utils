#include "../windoze.h"
#if WINDOWS
#define ZLIB_DLL 1
#endif
#include "../buffer.h"
#include <stdlib.h>

#ifdef HAVE_ZLIB
#include <zconf.h>
#include <zlib.h>
#undef ZEXPORT
#ifdef _WIN32
#define ZEXPORT __declspec(dllimport)
#else
#define ZEXPORT
#endif
ZEXPORT gzFile gzdopen(int, const char*);
ZEXPORT gzFile gzopen(const char*, const char*);
ZEXPORT int gzclose(gzFile);
ZEXPORT int gzflush(gzFile, int);
ZEXPORT int gzread(gzFile, voidp, unsigned);
ZEXPORT int gzwrite(gzFile, voidpc, unsigned);

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
  if((f = gzopen(filename, "rb")) == NULL)
    return -1;
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
  if((f = gzdopen(fd, "rb")) == NULL)
    return -1;
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
  gzFile f;
  char mode[4] = "wb0";
  mode[2] = '0' + (level % 10);
  if((f = gzopen(filename, mode)) == NULL)
    return -1;
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
  gzFile f;
  char mode[4] = "wb0";
  mode[2] = '0' + (level % 10);
  if((f = gzdopen(fd, mode)) == NULL)
    return -1;
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
