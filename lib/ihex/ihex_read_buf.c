#include "../ihex.h"
#include "../byte.h"

ssize_t
ihex_read_buf(ihex_file* ihf, const char* in, size_t n) {
  const char *x = in, *end = in + n;
  ihex_record** recp = &ihf->records;
  ssize_t ret = 0;

  *recp = 0;

  while(x < end) {
    size_t  len = byte_chrs(x, end - x, "\r\n", 2);

    ihex_read_data(recp, x, len);

    x += len;
    while(*x == '\r' || *x == '\n')
      ++x;

    recp = &((*recp)->next);
    ret++;

  }

  return ret;
}
