#include "../set.h"

/* Adds the values of other that set does not have. Returns how many were
 * added, or -1 if the memory ran out (what was added stays). */
int
set_cat(set_t* set, const set_t* other) {
  const bucket_t* b;
  int added = 0;

  if(set == other)
    return 0;

  for(b = other->list; b; b = b->list_next)
    switch(set_add(set, b->value, b->size)) {
      case SET_ADDED: ++added; break;
      case SET_ERROR: return -1;
    }

  return added;
}
