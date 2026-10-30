#include "../ihex.h"
#include "../byte.h"

ssize_t
ihex_read_buf(ihex_file* ihf, const char* in, size_t n) {
  const char *x = in, *end = in + n;
  ihex_recptr p, pos = {NULL};
  ssize_t ret = 0;

  pos.rec = (struct ihex_record_s*)&ihf->records;

  while(x < end) {
    size_t len = byte_chrs(x, end - x, "\r\n", 2);

    if(len && ihex_record_read(&p.rec, x, len) > 0) {
      list_add_tail(p.link, &ihf->records);
      pos.link = p.link;
      ret++;
    }

    x += len;

    while(x < end && (*x == '\r' || *x == '\n'))
      ++x;
  }

  return ret;
}
