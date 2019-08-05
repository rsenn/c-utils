#include "../byte.h"
#include "../stralloc.h"

void
stralloc_upper(register stralloc* sa) {
  byte_upper(sa->s, sa->len);
}

