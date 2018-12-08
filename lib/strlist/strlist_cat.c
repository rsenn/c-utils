#include "../strlist.h"

int
strlist_cat(strlist* sl, const strlist* l) {
  const char* x;
  size_t n;
  strlist_foreach(l, x, n) {
    strlist_pushb(sl, x, n);
  }
}
