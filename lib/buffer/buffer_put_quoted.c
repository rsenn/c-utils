#include "../buffer.h"
#include "../uint32.h"
#include "../fmt.h"

int
buffer_put_quoted(buffer* b, const char* x, size_t n) {
  size_t bytes = 0;

  for(size_t i = 0; i < n; i++) {
    size_t r;
    uint32 c = (unsigned int)(unsigned char)x[i];

    if(c < 0x20) {
      char buf[16];
      r = fmt_8long(buf, c);
      buffer_put(b, buf, r);
    } else {
      buffer_putc(b, c);
      r = 1;
    }

    bytes += r;
  }

  return bytes;
}
