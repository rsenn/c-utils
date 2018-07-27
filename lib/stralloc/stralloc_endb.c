#include "../byte.h"
#include "../byte.h"
#include "../stralloc.h"

unsigned int stralloc_endb(const stralloc *sa, const void *suffix, unsigned int len) {
  return (sa->len >= len) && byte_equal(sa->s + sa->len - len, len, suffix);
}
