#include "../textbuf.h"

void
textbuf_init(textbuf* tb, buffer* src) {

  tb->source = src;
  buffer_init(&tb->b, textbuf_read, tb, tb->buf, sizeof(tb->buf));
  tb->source = src;
  //tb->b.cookie = tb;
}
