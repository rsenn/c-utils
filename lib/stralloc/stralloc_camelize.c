#include "../stralloc.h"
#include "../byte.h"

size_t
stralloc_camelize(stralloc* sa) {
  sa->len = byte_camelize(sa->s, sa->len);
  return sa->len;
}

