#include "stralloc.h"

unsigned int stralloc_equalb(const stralloc *sa, const void *d, unsigned int dlen)
{
  return stralloc_diffb(sa, d, dlen) == 0;
}
