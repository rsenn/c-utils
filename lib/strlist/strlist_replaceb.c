#include "../strlist.h"

int
strlist_replaceb(strlist* l, const char* x, size_t n, const char* y, size_t len) {
  int64 i = strlist_indexofb(l, x, n);
  if(i != -1) {
    stralloc_replace(&l->sa, i, n, y, len);
    return 1;
  }
  return 0;
}
