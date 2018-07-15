#include "../byte.h"
#include "../stralloc.h"
#include "../str.h"

unsigned int stralloc_case_starts(const stralloc *sa, const char *prefix)
{
  register unsigned int prefix_len;
  prefix_len = str_len(prefix);
  if(sa->len < prefix_len) return 0;
  return byte_case_equal(sa->s, prefix_len, prefix);
}
