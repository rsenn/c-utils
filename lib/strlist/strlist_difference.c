#include "../strlist.h"

void
strlist_difference(const strlist* s1, const strlist* s2, strlist* out1, strlist* out2) {
  char* x;
  size_t n;
  strlist_foreach(s1, x, n) {
    if(!strlist_containsb(s2, x, n))
      strlist_pushb(out1, x, n);
  }
  strlist_foreach(s2, x, n) {
    if(!strlist_containsb(s1, x, n))
      strlist_pushb(out2, x, n);
  }
}
