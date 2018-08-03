#include "../strlist.h"

int
strlist_push_unique_sa(strlist* sl, const stralloc* sa) {
  if(strlist_contains_sa(sl, sa) != -1)
    return -1;

  return strlist_pushb(sl, sa->s, sa->len);
}
