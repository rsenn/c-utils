#include "../str.h"
#include "../stralloc.h"

extern int
stralloc_copy(stralloc* sa, const stralloc* sa2) {
  return stralloc_copyb(sa, sa2->s, sa2->len);
}
