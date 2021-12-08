#include "../set.h"
#include "../stralloc.h"

int
set_insertsa(set_t* set, const stralloc* sa) {
  return set_insert(set, sa->s, sa->len);
}
