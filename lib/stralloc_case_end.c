#include "byte.h"
#include "stralloc.h"

unsigned int stralloc_case_end(const stralloc *sa, const stralloc *suffix)
{
  register unsigned int sa_len;
  register unsigned int suffix_len;
  sa_len = sa->len;
  suffix_len = suffix->len;
  if (sa_len < suffix_len) return 0;
  return byte_case_equal(sa->s + sa_len - suffix_len, suffix_len, suffix->s);
}
