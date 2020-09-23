#include "../strlist.h"

int
strlist_push_unique_sa(strlist* sl, const stralloc* sa) {
  return strlist_pushb_unique(sl, sa->s, sa->len);
}
