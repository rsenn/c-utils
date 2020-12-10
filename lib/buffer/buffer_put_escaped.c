#include "../buffer.h"
#include "../byte.h"
#include "../fmt.h"
#include "../stralloc.h"
#include <ctype.h>

int
buffer_put_escaped(buffer* b, const char* x, size_t len, size_t (*escape)(char*, int)) {
  char buf[16];
  size_t i, n, r = 0;
  for(i = 0; i < len; i++) {
    n = escape(buf, x[i]);
    buffer_put(b, buf, n);
    r += n;
  }
  return r;
}
