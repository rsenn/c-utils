#include "../buffer.h"
#include "../uint32.h"

/* writes x[0..n-1], a control character (below 0x20) as a backslash and three
 * octal digits; returns the number of bytes written */
int
buffer_put_quoted(buffer* b, const char* x, size_t n) {
  size_t i;
  size_t bytes = 0;

  for(i = 0; i < n; ++i) {
    uint32 c = (unsigned int)(unsigned char)x[i];

    if(c < 0x20) {
      char buf[4];

      buf[0] = '\\';
      buf[1] = (char)('0' + ((c >> 6) & 7));
      buf[2] = (char)('0' + ((c >> 3) & 7));
      buf[3] = (char)('0' + (c & 7));
      buffer_put(b, buf, 4);
      bytes += 4;
    } else {
      buffer_putc(b, c);
      bytes += 1;
    }
  }

  return bytes;
}
