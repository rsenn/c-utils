#include "stralloc.h"

unsigned int stralloc_catint0(stralloc *sa, int i, unsigned int n)
{
  return stralloc_catlong0(sa, i, n);
}
