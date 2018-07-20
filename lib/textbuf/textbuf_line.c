#include "../textbuf.h"

unsigned
textbuf_line(buffer *b) {
  textbuf* tb = (void*)b->fd;
  return tb->line;
}

