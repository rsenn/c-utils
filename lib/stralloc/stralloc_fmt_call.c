#include "../windoze.h"
#include "../stralloc.h"
#include <stdarg.h>

typedef size_t(stralloc_fmt_x)(char*, void*, void*, void*, void*);

size_t
stralloc_fmt_call(stralloc* out, stralloc_fmt_fn* fmt_function, void* av[4]) {
  /*  va_list args;
    va_start(args, fmt_function);
    void* av[4];

    av[0] = va_arg(args, void*);
    av[1] = va_arg(args, void*);
    av[2] = va_arg(args, void*);
    av[3] = va_arg(args, void*);
  */
  ssize_t n;
  size_t i;

  stralloc_readyplus(out, 1024);
  n = (*((stralloc_fmt_x*)(void*)fmt_function))(
      &out->s[out->len], av[0], av[1], av[2], av[3]);
  out->len += n;

  stralloc_trunc(out, out->len);
  return n;
}
