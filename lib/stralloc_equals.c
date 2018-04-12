#include "stralloc.h"
#undef stralloc_equals

unsigned int stralloc_equals(const stralloc *sa, const char *s)
{
  return stralloc_diffs(sa, s) == 0;
}
