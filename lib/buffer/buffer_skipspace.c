#include "../buffer.h"
#include "../byte.h"

static int
is_space(int c) {
  return byte_chr(" \t\v\r\n\0", 6, c) < 6;
}

int
buffer_skipspace(buffer* b) {
  return buffer_skip_pred(b, is_space);
}

