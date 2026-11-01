#include "../set_internal.h"
#include <stdlib.h>
#include <string.h>

#define SET_MIN_SIZE 16

/* The one place where values enter a set. */
int
set_put(set_t* set, const void* val, size_t size, int prepend) {
  uint32 hash;
  bucket_t *b, *slot;
  char* copy;

  if(set->array == NULL) {
    if((set->array = calloc(SET_MIN_SIZE, sizeof(bucket_t))) == NULL)
      return SET_ERROR;

    set->len = SET_MIN_SIZE;
  }

  /* the copy has a terminating NUL, which the size does not count */
  if(size + 1 == 0)
    return SET_ERROR;

  hash = set_hash(set, val, size);

  if(set_lookup(set, val, size, hash))
    return SET_EXISTS;

  /* grow when the slots are three quarters full; if that fails the chains
   * still work, only longer */
  if((unsigned long long)(set->entries - set->overflow) * 4 >= (unsigned long long)set->len * 3 && set->len < 0x40000000u)
    set_rehash(set, set->len * 2);

  if((copy = malloc(size + 1)) == NULL)
    return SET_ERROR;

  memcpy(copy, val, size);
  copy[size] = '\0';

  slot = &set->array[hash & (set->len - 1)];

  if(!slot->value) {
    b = slot;
  } else {
    bucket_t* tail;

    if((b = malloc(sizeof(bucket_t))) == NULL) {
      free(copy);
      return SET_ERROR;
    }

    memset(b, 0, sizeof(bucket_t));

    for(tail = slot; tail->next; tail = tail->next) {
    }

    tail->next = b;
    ++set->overflow;
  }

  b->value = copy;
  b->size = size;
  b->hash = hash;

  if(prepend) {
    b->list_prev = NULL;
    b->list_next = set->list;

    if(set->list)
      set->list->list_prev = b;
    else
      set->last = b;

    set->list = b;
  } else {
    b->list_next = NULL;
    b->list_prev = set->last;

    if(set->last)
      set->last->list_next = b;
    else
      set->list = b;

    set->last = b;
  }

  ++set->entries;
  return SET_ADDED;
}
