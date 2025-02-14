#include "../set.h"
#include <stdlib.h>
#include <assert.h>

int
set_iterator_next(set_iterator_t* it) {
  const bucket_t* b;
  uint32 index;

  if(!(b = it->current))
    return 0;

  // check if there are overflowed buckets in our current position in the
  // array
  if(b->next) {
    it->current = b->next;
    return 1;
  }

  // no more buckets in our current index, so increment index
  // and seach for non-empty bucket
  index = it->index + 1;
  if(index >= it->set->len) {
    it->current = NULL;
    return 3;
  }

  b = &(it->set->array[index]);

  while(!b->value) {
    ++index;
    if(index >= it->set->len) {
      it->current = NULL;
      return 3;
    }
    b = &(it->set->array[index]);
  }

  it->current = b;
  it->index = index;

  return 1;
}
