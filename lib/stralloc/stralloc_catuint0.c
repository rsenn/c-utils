#include "stralloc.h"
#undef stralloc_catuint0

unsigned int stralloc_catuint0(stralloc *sa, unsigned int u, unsigned int n)
{
  return stralloc_catulong0(sa, u, n);
}
