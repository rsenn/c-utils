#include "../stralloc.h"
#include "../strlist.h"
#include "../byte.h"

void
strlist_intersection(const strlist* s1, const strlist* s2, strlist* out) {
  char* x;
  size_t n;

  if(s1 == out || s2 == out) {
    strlist sl;
    strlist_init(&sl, out->sep);
    strlist_intersection(s1, s2, &sl);
    strlist_free(out);
    *out = sl;
    return;
  }

  strlist_foreach(s1, x, n) {
    if(strlist_containsb(s2, x, n))
      strlist_pushb_unique(out, x, n);
  }
}
