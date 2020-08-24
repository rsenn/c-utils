#include "../buffer.h"
#include "../strarray.h"

int
buffer_putstra(buffer* b, const strarray* stra, const char* sep) {
  size_t i, n = strarray_size(stra);
  char** s = strarray_begin(stra);

  for(i = 0; i < n; ++i) {
    if(i > 0)
      buffer_puts(b, sep);
    buffer_puts(b, *s++);
  }

  return i;
}
