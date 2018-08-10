#include "../buffer.h"

#ifdef HAVE_LIBBZ2
#include <bzlib.h>

void
buffer_bzip_close(buffer* b) {
  BZFILE* f;
  if((f = (BZFILE*)b->fd)) {
    BZ2_bzflush(f);
    BZ2_bzclose(f);
  }
}

int
buffer_bunzip(buffer* b, const char* filename) {
  BZFILE* f;
  if((f = BZ2_bzopen(filename, "rb")) == NULL) return -1;
  b->fd = f;
  b->op = &BZ2_bzread;
  b->n = 
  b->p = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = malloc(b->a);
  b->deinit = buffer_bzip_close;
  return 0;
}

int
buffer_bzip(buffer* b, const char* filename, int level) {
  char mode[4] = { 'w', 'b', '0' + (level % 10), '\0' };
  BZFILE* f;
  if((f = BZ2_bzopen(filename, mode)) == NULL) return -1;
  b->fd = f;
  b->op = &BZ2_bzwrite;
  b->n = 
  b->p = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = malloc(b->a);
  b->deinit = buffer_bzip_close;
  return 0;
}
#endif
