#include "../stralloc.h"

size_t
stralloc_case_equals(const stralloc* sa, const char* s) {
  return stralloc_case_diffs(sa, s) == 0;
}

