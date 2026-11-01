#include "../set_internal.h"
#include <stdlib.h>
#include <string.h>

/* Takes the value out of the set, 1 if it was there, 0 if not. */
int
set_delete(set_t* set, const void* val, const size_t size) {
  bucket_t *head, *b, *prev = NULL;
  uint32 hash;

  if(set->array == NULL)
    return 0;

  hash = set_hash(set, val, size);
  head = &set->array[hash & (set->len - 1)];

  for(b = head; b && b->value; prev = b, b = b->next)
    if(size == b->size && b->hash == hash && memcmp(b->value, val, size) == 0)
      break;

  if(b == NULL || b->value == NULL)
    return 0;

  /* out of the list in the order of insertion */
  if(b->list_prev)
    b->list_prev->list_next = b->list_next;
  else
    set->list = b->list_next;

  if(b->list_next)
    b->list_next->list_prev = b->list_prev;
  else
    set->last = b->list_prev;

  free(b->value);

  if(prev) {
    /* an overflow bucket of its own */
    prev->next = b->next;
    free(b);
    --set->overflow;
  } else if(head->next) {
    /* the slot of the array is taken over by the first overflow bucket */
    bucket_t* n = head->next;

    *head = *n;
    free(n);
    --set->overflow;

    if(head->list_prev)
      head->list_prev->list_next = head;
    else
      set->list = head;

    if(head->list_next)
      head->list_next->list_prev = head;
    else
      set->last = head;
  } else {
    memset(head, 0, sizeof(bucket_t));
  }

  --set->entries;
  return 1;
}
