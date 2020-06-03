#include "../strlist.h"

size_t
strlist_diff(const strlist* a, const strlist* b, strlist* out) {
  const char* x;
  size_t count = 0, n;
  strlist_zero(out);
  strlist_foreach(a, x, n) {
    if(strlist_containsb(b, x, n))
      continue;
    strlist_pushb(out, x, n);
    count++;
  }
  return count;
}
