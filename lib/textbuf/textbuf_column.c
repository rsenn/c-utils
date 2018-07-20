#include "../textbuf.h"

unsigned
textbuf_column(buffer *b) {
  textbuf* tb = (void*)b->fd;
  return tb->column;
}

