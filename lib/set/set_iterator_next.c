#include "../set.h"

/* Moves the iterator to the next value. Returns 1, or 0 when there is none
 * (the iterator is then at the end and its value is NULL). */
int
set_iterator_next(set_iterator_t* it) {
  if(it->current) {
    it->current = it->current->list_next;
    ++it->index;
  }

  return it->current != NULL;
}
