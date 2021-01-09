#include "../buffer.h"
#include "../uint32.h"
#include "../fmt.h"

int
buffer_put_quoted(buffer* b, const char* x, size_t len) {
  char buf[16];
  size_t i, n, r = 0;
  for(i = 0; i < len; i++) {
    uint32 c = (unsigned int)(unsigned char)x[i];
    if(c < 0x20) {
      buffer_put(b, buf, (n = fmt_8long(buf, c)));
    } else {
      buffer_putc(b, c);
      n = 1;
    }
    r += n;
  }
  return r;
}
