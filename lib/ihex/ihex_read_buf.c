#include "../ihex.h"
#include "../byte.h"

ssize_t
ihex_read_buf(ihex_file* ihf, const char* in, size_t n) {
  const char *x = in, *end = in + n;
  ihex_recptr p, pos = {&ihf->records};
  ssize_t ret = 0;

  while(x < end) {
    size_t len = byte_chrs(x, end - x, "\r\n", 2);

    ihex_record_read(&p.rec, x, len);

    list_add_tail(p.link, &ihf->records);
    pos.link = p.link;

    x += len;
    while(*x == '\r' || *x == '\n')
      ++x;

    ret++;
  }

  return ret;
}
