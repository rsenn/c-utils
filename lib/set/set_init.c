#include "../set.h"
#include <stdlib.h>
#include <assert.h>

#define BASE_SIZE 1024

void
set_init(set_t* set, uint32 (*const hash_fp)(const void*, size_t)) {

  assert(set);

  set->entries = 0;
  set->overflow = 0;
  set->hash_fp = hash_fp ? hash_fp : &set_hashfunc;
  set->len = BASE_SIZE;
  set->array = calloc(set->len, sizeof(bucket_t));
  assert(set->array);
}
