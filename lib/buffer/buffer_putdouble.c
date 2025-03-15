#include "../buffer.h"
#include "../byte.h"
#include "../fmt.h"
#include "../str.h"
#include <float.h>

int
buffer_putdouble(buffer* b, double d, int prec) {
  char x[DBL_DIG + 2];
  size_t n = fmt_double(x, d, sizeof(x), prec);

  if(byte_chr(x, n, '.') < n) {
    while(n > 0 && x[n - 1] == '0') {
      if(n > 1 && x[n - 2] == '.')
        break;

      n--;
    }
  }

  return buffer_put(b, x, n);
}
