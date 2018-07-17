#include "../stralloc.h"
#undef stralloc_catint

unsigned int stralloc_catint(stralloc *sa, int i) {
  return stralloc_catlong0(sa, i, 0);
}
