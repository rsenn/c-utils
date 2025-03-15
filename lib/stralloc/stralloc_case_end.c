#include "../byte.h"
#include "../stralloc.h"

size_t
stralloc_case_end(const stralloc* sa, const stralloc* suffix) {
  size_t sa_len = sa->len, suffix_len = suffix->len;

  if(sa_len < suffix_len)
    return 0;

  return byte_case_equal(sa->s + sa_len - suffix_len, suffix_len, suffix->s);
}
