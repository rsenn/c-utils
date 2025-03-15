#include "../buffer.h"
#include "../str.h"

int
buffer_putns(buffer* b, const char* s, int ntimes) {
  size_t n = str_len(s);

  while(ntimes-- > 0)
    if(buffer_put(b, s, n) < 0)
      return -1;

  return 0;
}
