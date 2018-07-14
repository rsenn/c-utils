#include "stralloc.h"

unsigned int stralloc_case_equal(const stralloc *sa1, const stralloc *sa2)
{
  return stralloc_case_diff(sa1, sa2) == 0;
}
