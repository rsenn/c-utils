#include "../set.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void set_free_array(set_t* set);

static int
set_realloc(set_t* set) {
  uint32 set_index;
  bucket_t* b;

  // allocate new set
  set_t new_set;

  new_set.entries = 0;
  new_set.overflow = 0;
  new_set.hash_fp = set->hash_fp;
  new_set.len = set->len * 2;
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
  set->entries = new_set.entries;
  set->overflow = new_set.overflow;
  set->hash_fp = new_set.hash_fp;
  set->len = new_set.len;
  set->array = new_set.array;
  return 1;
}

int
set_add(set_t* set, const void* val, const size_t size) {
  uint32 hash, index;
  bucket_t* b;

  if(set->entries == 0 && set->len == 0 && set->array == 0 && set->hash_fp == 0)
    set_init(set, 0);

  if(set_has(set, val, size))
    return 2;

  if((set->entries - set->overflow) > (set->len * .75))
    assert(set_realloc(set) == 1);

  hash = set->hash_fp(val, size);
  index = hash & (set->len - 1);
  b = &(set->array[index]);

  if(!b->value) {
    b->hash = hash;
    b->value = malloc(size);
    assert(b->value);
    memcpy(b->value, val, size);
    b->size = size;
  } else {
    if(b->next == NULL) {
      b->next = malloc(sizeof(bucket_t));
      assert(b->next);
      b->next->hash = hash;
      b->next->next = NULL;
      b->next->value = malloc(size);
      assert(b->next->value);
      memcpy(b->next->value, val, size);
      b->next->size = size;
      ++set->overflow;
    } else {
      //    bucket_t* b = set->array[index].next;
      while(b->next) b = b->next;

      b->next = malloc(sizeof(bucket_t));
      assert(b->next);
      b->next->hash = hash;
      b->next->next = NULL;
      b->next->value = malloc(size);
      assert(b->next->value);
      memcpy(b->next->value, val, size);
      b->next->size = size;

      ++set->overflow;
    }
  }

  ++set->entries;
  return 1;
}
