#include "../buffer.h"

int
buffer_skipn(buffer* b, size_t n) {
  if(n > buffer_LEN(b))
    if(buffer_prefetch(b, n) < n)
      return 0;

  if(n <= buffer_LEN(b)) {
    buffer_SKIP(b, n);
    return n;
  }

  return 0;
}
