#include "../buffer.h"
#include "../str.h"

int
buffer_puts_escaped(buffer* b, const char* x) {
  return buffer_put_escaped(b, x, str_len(x));
}

