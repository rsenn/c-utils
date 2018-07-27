#include "../buffer.h"
#include "../fmt.h"

int
buffer_putulong0(buffer* b, unsigned long l, int pad) {
  char buf[FMT_ULONG+pad];
  return buffer_put(b, buf, fmt_ulong0(buf, l, pad));
}
