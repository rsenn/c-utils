#include "../byte.h"
#include "../byte.h"
#include "../stralloc.h"

unsigned int stralloc_ends(const stralloc *sa, const stralloc *suffix) {
  unsigned int suffix_len;
  suffix_len = suffix->len;
  return (sa->len >= suffix_len) && byte_equal(sa->s + sa->len - suffix_len, suffix_len, suffix->s);
}
