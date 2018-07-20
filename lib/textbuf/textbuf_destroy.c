#include "../textbuf.h"

void
textbuf_destroy(buffer* b) {
  textbuf* tb = (void*)b->fd;
  free(tb);
}


