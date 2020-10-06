#include "../set.h"
#include "../stralloc.h"

int
set_has_sa(const set_t* set, const stralloc* sa) {
  return set_has(set, sa->s, sa->len);
}