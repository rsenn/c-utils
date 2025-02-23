#include "../strlist.h"

int
strlist_pushb_unique(strlist* sl, const char* s, size_t n) {
  if(strlist_indexofb(sl, s, n) != -1)
    return 0;

  strlist_pushb(sl, s, n);
  return 1;
}
