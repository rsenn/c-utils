#include "../buffer.h"
#include "../byte.h"
#include "../fmt.h"
#include "../str.h"
#include <float.h>

int
buffer_putdouble(buffer* b, double d, int prec) {
  char buf[DBL_DIG + 2];
  size_t n = fmt_double(buf, d, sizeof(buf), prec);
  if(byte_chr(buf, n, '.') < n) {
    while(n > 0 && buf[n - 1] == '0') {
      if(n > 1 && buf[n - 2] == '.')
        break;
      n--;
    }
  }
  return buffer_put(b, buf, n);
}
