#include "../byte.h"
#include "../str.h"
#include "../stralloc.h"

int
stralloc_case_diffs(const stralloc* sa, const char* s) {
  int r;
  unsigned int s_len = str_len(s);

  /* get shortest len */
  size_t len = sa->len;

  if(len > s_len)
    len = s_len;

  /* compare common lengths */
  if((r = byte_case_diff(sa->s, len, s)))
    return r;

  return (int)sa->len - (int)s_len;
}
