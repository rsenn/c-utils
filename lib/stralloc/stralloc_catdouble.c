#include "../fmt.h"
#include "../stralloc.h"

int
stralloc_catdouble(stralloc* sa, double d, int prec) {
  char buf[64];

  return stralloc_catb(sa, buf, fmt_double(buf, d, sizeof(buf), prec));
}
