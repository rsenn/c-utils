#include "../strlist.h"

int
strlist_push_unique_sa(strlist* sl, const stralloc* sa) {
  if(strlist_contains_sa(sl, sa))
    return 0;
  return strlist_push_sa(sl, sa);
}
