#include "../stralloc.h"

int
stralloc_splice(stralloc* sa, size_t start, size_t ndel, size_t nadd, const char* x) {
  ssize_t diff = nadd - ndel;
  char* x = sa->s;
  size_t p = start + ndel;
  size_t n = sa->len - p;
  if(diff > 0)
    if(!stralloc_readyplus(sa, diff))
      return 0;
  (diff > 0 ? byte_copyr : byte_copy)(&x[start + nadd], n, &x[p]);
  if(nadd > 0)
    byte_copy(&x[start], nadd, x);
  sa->len += diff;
  return 1;
}
