#include "../stralloc.h"
#undef stralloc_equals

size_t
stralloc_equals(const stralloc* sa, const char* s) {
  return stralloc_diffs(sa, s) == 0;
}
