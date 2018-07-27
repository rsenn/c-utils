#include "../textbuf.h"

uint64
textbuf_column(buffer *b) {
  textbuf* tb = is_textbuf(b);
  return tb ? tb->column : 0;
}

