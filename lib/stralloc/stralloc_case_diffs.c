#include "../byte.h"
#include "../str.h"
#include "../stralloc.h"

int
stralloc_case_diffs(const stralloc* sa, const char* s) {
  size_t len;
  unsigned int s_len;
  int r;

  s_len = str_len(s);
  /*
    get shortest len
  */
  len = sa->len;
  if(len > s_len) {
    len = s_len;
  }
  /*
    compare common lengths
  */
  r = byte_case_diff(sa->s, len, s);
  if(r)
    return r;
  return (int)sa->len - (int)s_len;
}
