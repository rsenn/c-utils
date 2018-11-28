#include "../byte.h"
#include "../stralloc.h"

size_t
stralloc_case_start(const stralloc* sa, const stralloc* prefix) {
  unsigned int prefix_len;
  prefix_len = prefix->len;
  if(sa->len < prefix_len)
    return 0;
  return byte_case_equal(sa->s, prefix_len, prefix->s);
}
