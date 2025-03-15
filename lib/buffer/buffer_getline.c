#include "../buffer.h"

ssize_t
buffer_getline(buffer* b, char* x, size_t n) {
  return buffer_get_token(b, x, n, "\n", 1);
}
