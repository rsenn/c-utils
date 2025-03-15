#include "../buffer.h"
#include "../uint32.h"
#include <stdarg.h>

typedef size_t format_function(char*, int, void*, void*, void*, void*);

int
buffer_putfmt_args(buffer* b, const char* x, size_t n, format_function* escape, void* args[]) {
  char buf[16];
  size_t r = 0;

  for(size_t i = 0; i < n; i++) {
    uint32 c = (unsigned int)(unsigned char)x[i];
    size_t bytes = escape(buf, c, args[0], args[1], args[2], args[3]);

    buffer_put(b, buf, bytes);
    r += bytes;
  }

  return r;
}

int
buffer_putfmt_va(buffer* b, const char* x, size_t n, size_t (*escape)(char*, int), va_list args) {
  void* av[4];

  av[0] = va_arg(args, void*);
  av[1] = va_arg(args, void*);
  av[2] = va_arg(args, void*);
  av[3] = va_arg(args, void*);

  return buffer_putfmt_args(b, x, n, (format_function*)escape, av);
}

int
buffer_putfmt(buffer* b, const char* x, size_t n, size_t (*escape)(char*, int), ...) {
  va_list args;
  int ret;

  va_start(args, escape);
  ret = buffer_putfmt_va(b, x, n, escape, args);
  va_end(args);

  return ret;
}
