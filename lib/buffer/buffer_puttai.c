#include "../buffer.h"
#include "../tai.h"
#include "../fmt.h"

int
buffer_puttai(buffer* b, const tai64* t) {
  char buf[FMT_ISO8601];
  return buffer_put(b, buf, fmt_tai(buf, t));
}

