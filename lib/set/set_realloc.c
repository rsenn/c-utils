#include "../set.h"
#include <stdlib.h>
#include <assert.h>

void set_free_array(set_t* set);

int
set_realloc(set_t* set) {
  bucket_t* b;

  // allocate new set
  set_t new_set = {0, 0};

  new_set.len = set->len * 2;
  new_set.hash_fp = set->hash_fp;
  new_set.array = calloc(new_set.len, sizeof(bucket_t));
  assert(new_set.array);

  // copy over old set, preserving insertion order (set_foreach_ordered
  // relies on it) -- walk the old list/list_next chain, not the bucket
  // array, which is in hash order rather than insertion order

  for(b = set->list; b; b = b->list_next)
    set_add(&new_set, b->value, b->size);

  // delete old array
  set_free_array(set);

  // set new fields
  *set = new_set;
  return 1;
}
