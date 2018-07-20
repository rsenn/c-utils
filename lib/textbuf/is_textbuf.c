#include "../textbuf.h"

int
is_textbuf(buffer* b) {
  return (char*)b->fd == b->x - (size_t)((char*)&(((textbuf*)0)->buf));
}

