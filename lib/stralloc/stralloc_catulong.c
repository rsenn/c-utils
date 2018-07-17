#include "../stralloc.h"

unsigned int stralloc_catulong(register stralloc *sa, unsigned long ul) {
  return stralloc_catulong0(sa, ul, 0);
}
