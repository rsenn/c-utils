#include "../strlist.h"

int
strlist_removeb(strlist* l, const char* x, size_t n) {
  int64 i = strlist_indexofb(l, x, n);
  if(i != -1) {
    stralloc_remove(&l->sa, i, n);
    return 1;
  }
  return 0;
}
