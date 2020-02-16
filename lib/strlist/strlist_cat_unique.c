#include "../strlist.h"

int
strlist_cat_unique(strlist* sl, const strlist* l) {

  const char* x;
  size_t n;
  int ret = 0;

  strlist_foreach(l, x, n) {
    if(!strlist_containsb(sl, x, n)) {
      strlist_pushb(sl, x, n);
      ret++;
    }
  }
  return ret;
}