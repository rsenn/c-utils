#include "../textbuf.h"

ssize_t
textbuf_read(int fd, char* x, size_t n, void* buf) {
  textbuf* tb = is_textbuf(buf); 
  ssize_t r = buffer_get(tb->source, x, n);

  for(ssize_t i = 0; i < r; ++i) {
    if(x[i] == '\n') {
      ++tb->line;
      tb->column = 0;
    } else {
      ++tb->column;
    }
  }
  return r;
}
