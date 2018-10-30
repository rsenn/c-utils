#include "../strlist.h"

int
strlist_removeb(strlist* l, const char* x, size_t n) {
  int64 i = strlist_indexofb(l, x, n);
  if(i != -1) {
    if(i + n < l->sa.len)
      if(l->sa.s[i + n] == l->sep)
        ++n;

    stralloc_remove(&l->sa, i, n);
    return 1;
  }
  return 0;
}
