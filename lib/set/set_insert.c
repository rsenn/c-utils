#include "../set.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int set_realloc(set_t* set);

int
set_insert(set_t* set, const void* val, const size_t size) {
  uint32 hash, index;
  bucket_t* b;

  if(set->entries == 0 && set->len == 0 && set->array == 0 && set->hash_fp == 0)
    set_init(set, 0);

  if(set_has(set, val, size))
    return 2;

  if((set->entries - set->overflow) > (set->len * .75)) {
    int r = set_realloc(set);
    assert(r == 1);
  }

  hash = set->hash_fp(val, size);
  index = hash & (set->len - 1);
  b = &(set->array[index]);

  if(!b->value) {
    b->hash = hash;
    b->value = calloc(1, size + 1);
    assert(b->value);
    memcpy(b->value, val, size);
    b->size = size;

  } else {
    if(b->next == NULL) {
      b->next = malloc(sizeof(bucket_t));
      assert(b->next);
      b->next->hash = hash;
      b->next->next = NULL;
      b->next->value = calloc(1, size + 1);
      assert(b->next->value);
      memcpy(b->next->value, val, size);
      b->next->size = size;
      ++set->overflow;
    } else {
      //    bucket_t* b = set->array[index].next;
      while(b->next)
        b = b->next;

      b->next = malloc(sizeof(bucket_t));
      assert(b->next);
      b->next->hash = hash;
      b->next->next = NULL;
      b->next->value = calloc(1, size + 1);
      assert(b->next->value);
      memcpy(b->next->value, val, size);
      b->next->size = size;

      ++set->overflow;
    }

    b = b->next;
  }

  b->list_next = set->first;
  b->list_prev = 0;

  if(set->first)
    set->first->list_prev = b;
  else
    set->list = b;

  set->first = b;

  ++set->entries;
  return 1;
}
