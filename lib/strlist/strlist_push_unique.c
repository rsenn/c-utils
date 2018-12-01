#include "../strlist.h"

int
strlist_push_unique(strlist* sl, const char* s) {
  if(strlist_index_of(sl, s) != -1)
    return 0;
  strlist_push(sl, s);
  return 1;
}