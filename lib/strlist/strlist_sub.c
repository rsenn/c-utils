#include "../strlist.h"

int
strlist_sub(strlist* l, const strlist* o) {
  int r = 0;
  char* x;
  size_t n;
  strlist_foreach(o, x, n) {
    if(strlist_removeb(l, x, n))
      r++;
  }
  return r;
}
