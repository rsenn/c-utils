#include "../set.h"
#include "../stralloc.h"

int
set_addsa(set_t* set, const stralloc* sa) {
  return set_add(set, sa->s, sa->len);
}
