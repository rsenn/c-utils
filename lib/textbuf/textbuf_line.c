#include "../textbuf.h"

uint64
textbuf_line(buffer *b) {
  textbuf* tb = is_textbuf(b);
  return tb ? tb->line : 0;
}

