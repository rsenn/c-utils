#include "../byte.h"
#include "../str.h"
#include "../byte.h"
#include "../stralloc.h"

unsigned int stralloc_ends(register const stralloc *sa, const char *suffix)
{
  register unsigned int suffix_len;
  suffix_len = str_len(suffix);
  return (sa->len >= suffix_len) && byte_equal(sa->s + sa->len - suffix_len, suffix_len, suffix);
}
