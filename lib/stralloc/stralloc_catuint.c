#include "../stralloc.h"

unsigned int stralloc_catuint(stralloc *sa, unsigned int ui) {
  return stralloc_catulong0(sa, ui, 0);
}
