#include "../byte.h"
#include "../byte.h"
#include "../stralloc.h"

unsigned int stralloc_start(register const stralloc *sa, register const stralloc *prefix) {
  register unsigned int len;
  len = prefix->len;
  return (sa->len >= len) && byte_equal(prefix->s, len, sa->s);
}
