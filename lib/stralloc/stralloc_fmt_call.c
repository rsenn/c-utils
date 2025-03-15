#include "../windoze.h"
#include "../stralloc.h"
#include <stdarg.h>

typedef size_t(stralloc_fmt_x)(char*, void*, void*, void*, void*);

size_t
stralloc_fmt_call(stralloc* out, stralloc_fmt_fn* fn, void* av[4]) {
  ssize_t n;

  stralloc_readyplus(out, 1024);
  n = (*((stralloc_fmt_x*)(void*)fn))(stralloc_end(out), av[0], av[1], av[2], av[3]);
  out->len += n;

  stralloc_trunc(out, out->len);
  return n;
}
