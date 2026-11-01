#include "../buffer.h"
#include "../uint32.h"
#include <stdarg.h>
typedef size_t format_function(char*, int, void*, void*, void*, void*);

int
buffer_putfmt(buffer* b, const char* x, size_t n, size_t (*escape)(char*, int), ...) {
  va_list args;
  int ret;

  va_start(args, escape);
  ret = buffer_putfmt_va(b, x, n, escape, args);
  va_end(args);

  return ret;
}
