#include "../byte.h"
#include "../stralloc.h"

void
stralloc_lower(stralloc* sa) {
  byte_lower(sa->s, sa->len);
}

