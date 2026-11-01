#include "../set.h"

/* Adds the values that both a and b have to out, in the order of a. Returns
 * how many values were added to out, or -1 if the memory ran out. */
int
set_intersection(set_t* out, const set_t* a, const set_t* b) {
  const bucket_t* x;
  int added = 0;

  for(x = a->list; x; x = x->list_next)
    if(set_has(b, x->value, x->size))
      switch(set_add(out, x->value, x->size)) {
        case SET_ADDED: ++added; break;
        case SET_ERROR: return -1;
      }

  return added;
}
