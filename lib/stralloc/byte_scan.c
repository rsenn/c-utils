#include "../byte.h"
#include "../stralloc.h"
#include <stdarg.h>

typedef size_t scan_call_t(const char*, void*, void*, void*, void*, void*);

size_t
byte_scan(const char* in, size_t in_len, stralloc* out, size_t (*scan_function)(), ...) {
  va_list a;
  void* v[4];

  va_start(a, scan_function);

  v[0] = va_arg(a, void*);
  v[1] = va_arg(a, void*);
  v[2] = va_arg(a, void*);
  v[3] = va_arg(a, void*);

  stralloc_zero(out);

  for(size_t i = 0; i < in_len;) {
    if(!stralloc_readyplus(out, 10))
      return 0;

    i += ((scan_call_t*)scan_function)(&in[i], stralloc_end(out), v[0], v[1], v[2], v[3]);
    ++out->len;
  }

  return out->len;
}
