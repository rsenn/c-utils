#include "../set.h"

/* Puts the iterator at the first value (in the order of insertion). Returns 1,
 * or 0 if the set is empty. */
int
set_iterator_init(set_iterator_t* it, const set_t* set) {
  it->index = 0;
  it->set = set;
  it->current = set ? set->list : NULL;
  return it->current != NULL;
}
