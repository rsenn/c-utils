#include "../buffer.h"
#include "../uint32.h"

int
buffer_put_escaped(buffer* b, const char* x, size_t len, size_t (*escape)(char*, int)) {
  char buf[16];
  size_t i, n, r = 0;
  for(i = 0; i < len; i++) {
    uint32 c = (unsigned int)(unsigned char)x[i];
    n = escape(buf, c);
    buffer_put(b, buf, n);
    r += n;
  }
  return r;
}
