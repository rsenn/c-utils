#include "../buffer.h"

#ifdef HAVE_ZLIB
#include <zlib.h>

void
buffer_gz_close(buffer* b) {
  gzFile f;
  if((f = (gzFile)b->fd)) {
    gzflush(f, Z_FINISH);
    gzclose(f);
  }
}

int
buffer_gunzip(buffer* b, const char* filename) {
  gzFile f;
  if((f = gzopen(filename, "rb")) == NULL) return -1;
  b->fd = f;
  b->op = &gzread;
  b->n = 
  b->p = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = malloc(b->a);
  b->deinit = buffer_gz_close;
  return 0;
}

int
buffer_gzip(buffer* b, const char* filename, int level) {
  char mode[4] = { 'w', 'b', '0' + (level % 10), '\0' };
  gzFile f;
  if((f = gzopen(filename, mode)) == NULL) return -1;
  b->fd = f;
  b->op = &gzwrite;
  b->n = 
  b->p = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = malloc(b->a);
  b->deinit = buffer_gz_close;
  return 0;
}
#endif
