#include "../byte.h"
#include "../str.h"
#include "../stralloc.h"

unsigned int
stralloc_case_starts(const stralloc* sa, const char* prefix) {
  unsigned int prefix_len;
  prefix_len = str_len(prefix);
  if(sa->len < prefix_len)
    return 0;
  return byte_case_equal(sa->s, prefix_len, prefix);
}
