#include "../stralloc.h"

unsigned int
stralloc_catulong(stralloc* sa, unsigned long int ul) {
  return stralloc_catulong0(sa, ul, 0);
}
