#include "../set.h"
#include <stdlib.h>
#include <assert.h>

/*
 * Helper function that finds first non-empty bucket and inits
 * the iterator accordingly
 *
 */
static int
init_bucket(set_iterator_t* it) {
  bucket_t* b = it->set->array;
  uint32 index = 0;

  if(!b)
    return 0;

  while(!b->value) {
    ++index;

    if(index >= it->set->len)
      return 0;
    b = &(it->set->array[index]);
  }
  it->current = b;
  it->index = index;

  return 1;
}

int
set_iterator_init(set_iterator_t* it, const set_t* set) {
  if(!set)
    return 0;
  assert(it);
  it->index = 0;
  it->current = NULL;
  it->set = set;
  return init_bucket(it) == 1;
}
