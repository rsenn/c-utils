#include "../sse.h"
#include "../byte.h"

void
sse_init(sse* s) {
  byte_zero(s, sizeof(sse));
  s->retry = -1;
}
