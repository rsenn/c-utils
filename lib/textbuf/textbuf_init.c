#include "../textbuf.h"

void
textbuf_init(textbuf* tb, buffer* src) {

  tb->source = src;
  buffer_init(&tb->b, textbuf_read, -1, NULL, 0);
}
