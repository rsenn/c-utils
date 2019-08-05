#include "../textbuf.h"

ssize_t
textbuf_read(fd_t fd, char* x, size_t n, void* buf) {
  textbuf* tb = is_textbuf(buf);
  ssize_t i, r = buffer_get(tb->source, x, n);

  for(i = 0; i < r; ++i) {
    if(x[i] == '\n') {
      ++tb->line;
      tb->column = 0;
    } else {
      ++tb->column;
    }
  }
  return r;
}

