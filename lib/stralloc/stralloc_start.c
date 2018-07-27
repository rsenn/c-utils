#include "../byte.h"
#include "../byte.h"
#include "../stralloc.h"

unsigned int stralloc_start(const stralloc *sa, const stralloc *prefix) {
  unsigned int len;
  len = prefix->len;
  return (sa->len >= len) && byte_equal(prefix->s, len, sa->s);
}
