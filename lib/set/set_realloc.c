#include "../set.h"
#include <stdlib.h>
#include <assert.h>

void set_free_array(set_t* set);

int
set_realloc(set_t* set) {
  uint32 set_index;
  bucket_t* b;

  // allocate new set
  set_t new_set = {0, 0};

  new_set.len = set->len * 2;
  new_set.hash_fp = set->hash_fp;
  new_set.array = calloc(new_set.len, sizeof(bucket_t));
  assert(new_set.array);

  // copy over old set

  for(set_index = 0; set_index < set->len; ++set_index) {
    b = &(set->array[set_index]);

    while(b && b->value) {
      set_add(&new_set, b->value, b->size);
      b = b->next;
    }
  }

  // delete old array
  set_free_array(set);

  // set new fields
  *set = new_set;
  return 1;
}
