#include "../buffer.h"
#include "../uint32.h"
#include <stdarg.h>
typedef size_t format_function(char*, int, void*, void*, void*, void*);

int
buffer_putfmt_args(buffer* b, const char* x, size_t n, format_function* escape, void* args[]) {
  size_t i;
  char buf[16];
  size_t r = 0;

  for(i = 0; i < n; i++) {
    uint32 c = (unsigned int)(unsigned char)x[i];
    size_t bytes = escape(buf, c, args[0], args[1], args[2], args[3]);

    buffer_put(b, buf, bytes);
    r += bytes;
  }

  return r;
}
