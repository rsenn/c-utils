#include "../textbuf.h"

ssize_t
textbuf_read(intptr_t fd, char* x, size_t n) {
  textbuf* tb = (void*)fd;
  ssize_t r = buffer_get(tb->source, x, n);

  for(ssize_t i = 0; i < r; ++i) {
    if(x[i] == '\n') {
      ++tb->line;
      tb->col = 0;
    } else {
      ++tb->col;
    }
  }
  return r;
}
