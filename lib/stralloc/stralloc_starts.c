#include "../stralloc.h"
#include "../byte.h"
#include "../str.h"

int
stralloc_starts(stralloc *sa, const char *in) {
  size_t len = str_len(in);
  return (len <= sa->len && !byte_diff(sa->s, len, in));
}

