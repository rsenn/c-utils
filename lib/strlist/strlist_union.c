#include "../strlist.h"

void
strlist_union(const strlist* s1, const strlist* s2, strlist* out) {
  char* x;
  size_t n;
  strlist_foreach(s1, x, n) { strlist_pushb_unique(out, x, n); }
  strlist_foreach(s2, x, n) { strlist_pushb_unique(out, x, n); }
}
