#include "../stralloc.h"
#undef stralloc_catint0

unsigned int stralloc_catint0(stralloc *sa, int i, unsigned int n) {
  return stralloc_catlong0(sa, i, n);
}
