#include "../buffer.h"
#include "../uint32.h"
#include <stdarg.h>
typedef size_t format_function(char*, int, void*, void*, void*, void*);

int
buffer_putfmt_va(buffer* b, const char* x, size_t n, size_t (*escape)(char*, int), va_list args) {
  void* av[4];

  av[0] = va_arg(args, void*);
  av[1] = va_arg(args, void*);
  av[2] = va_arg(args, void*);
  av[3] = va_arg(args, void*);

  return buffer_putfmt_args(b, x, n, (format_function*)escape, av);
}
