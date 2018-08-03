#include <float.h>
#include "../buffer.h"
#include "../fmt.h"
#include "../str.h"

int
buffer_putdouble(buffer* b, double d, int prec) {
  char buf[DBL_DIG+2];
  size_t n = fmt_double(buf, d, sizeof(buf), prec);

  while(n > 1 && str_chr("0.", buf[n - 1]) < 2) {
    --n;
    if(buf[n] == '.') break;
  }

  return buffer_put(b, buf, n);
}
