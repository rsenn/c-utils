#include "../byte.h"
#include "../byte.h"
#include "../stralloc.h"

unsigned int stralloc_startb(register const stralloc *sa, const void *prefix, register unsigned int len) {
  return (sa->len >= len) && byte_equal(sa->s, len, prefix);
}
