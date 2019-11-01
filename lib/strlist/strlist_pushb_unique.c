#include "../strlist.h"

int
strlist_pushb_unique(strlist* sl, const char* x, size_t n) {
  if(strlist_indexofb(sl, x, n) != -1)
    return 0;
  return strlist_pushb(sl, x, n);
}
