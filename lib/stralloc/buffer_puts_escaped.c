#include "../buffer.h"
#include "../str.h"
#include "../fmt.h"

int
buffer_puts_escaped(buffer* b, const char* x, size_t (*escape)(char*, int)) {
  void* args[4] = {0, 0, 0, 0};

  return buffer_putfmt_args(b, x, str_len(x), (size_t(*)(char*, int, void*, void*, void*, void*))escape, args);
}
