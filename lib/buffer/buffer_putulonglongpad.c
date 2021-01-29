#include "../buffer.h"
#include "../fmt.h"
#include <stdlib.h>

int
buffer_putulonglongpad(buffer* b, uint64 l, int pad) {
  char buf[FMT_ULONG];
  size_t n = fmt_ulonglong(buf, l);

  if(buffer_putnc(b, ' ', pad - n) < 0)
    return -1;

  return buffer_put(b, buf, n);
}
