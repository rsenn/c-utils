#include <stdlib.h>
#include "../textbuf.h"
#include "../byte.h"

void
textbuf_init(buffer* b, buffer* src, size_t bufsize) {
  textbuf* tb = malloc(sizeof(textbuf) + bufsize);

  if(tb) {
    byte_zero(tb, sizeof(textbuf));
    tb->source = src;
    buffer_init(b, (buffer_op_fn*)&textbuf_read, -1, tb->buf, bufsize);
    b->deinit = &textbuf_free;
    b->cookie = tb;
  }
}
