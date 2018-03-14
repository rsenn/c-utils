#include "stralloc.h"

unsigned int stralloc_equal(const stralloc *sa1, const stralloc *sa2)
{
  return stralloc_diff(sa1, sa2) == 0;
}
