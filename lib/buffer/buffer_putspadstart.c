#include "../buffer.h"
#include "../str.h"

int
buffer_putspadstart(buffer* b, const char* x, size_t pad) {
  size_t n = str_len(x), alen = str_ansilen(x);

  if(alen < pad)
    if(buffer_putnspace(b, pad - alen) < 0)
      return -1;

  if(n > 0)
    if(buffer_put(b, x, n) < 0)
      return -1;

  return 0;
}
