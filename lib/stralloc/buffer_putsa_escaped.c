#include "../stralloc.h"
#include "../buffer.h"
#include "../str.h"
#include "../fmt.h"

int
buffer_put_escaped(buffer* b, const void* x, size_t len, size_t (*escape)(char*, int)) {
  void* args[4] = {0, 0, 0, 0};
  return buffer_putfmt_args(b, x, len, (size_t(*)(char*, int, void*, void*, void*, void*))escape, args);
}

int
buffer_putsa_escaped(buffer* b, const stralloc* sa, size_t (*escape)(char*, int)) {
  void* args[4] = {0, 0, 0, 0};
  return buffer_putfmt_args(b, sa->s, sa->len, (size_t(*)(char*, int, void*, void*, void*, void*))escape, args);
}
