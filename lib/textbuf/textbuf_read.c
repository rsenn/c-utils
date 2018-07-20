#include "../textbuf.h"

ssize_t
textbuf_read(intptr_t fd, void* buf, size_t n, void* cookie) {
  textbuf* tb = (void*)fd;
  char* x = buf;
  ssize_t r = buffer_get(tb->source, buf, n);

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
