#include "../buffer.h"
#include "../byte.h"
#include "../fmt.h"
#include "../str.h"
#include <float.h>

int
buffer_putdouble(buffer* b, double d, int prec) {
  char buf[DBL_DIG + 2];
  return buffer_put(b, buf, fmt_double(buf, d, sizeof(buf), prec));
}
