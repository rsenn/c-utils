#include "../ihex.h"

ssize_t
ihex_read_buf(ihex_file* ihf, const char* in, size_t n) {
  const char *x = in, *end = in + n;
  ihex_record* rec = 0;

  while(x < end) {
    size_t  len = byte_chrs(x, end - x, "\r\n", 2);

    ihex_read_data(&rec, x, len);

    x += len;
    while(*x == '\r' || *x == '\n')
      ++x;
  }
}
