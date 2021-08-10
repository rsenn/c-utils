#include "../strlist.h"

size_t
strlist_remove_all(strlist* sl, const strlist* remove) {
  const char* x;
  size_t count = 0, n;
  strlist_foreach(remove, x, n) {
    /*  if(strlist_containsb(sl, x, n)) */ {
      strlist_removeb(sl, x, n);
      count++;
    }
  }

  return count;
}
