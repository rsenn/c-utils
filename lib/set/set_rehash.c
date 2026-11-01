#include "../set_internal.h"
#include <stdlib.h>
#include <string.h>

static void
free_chains(bucket_t* array, uint32 len) {
  uint32 i;

  for(i = 0; i < len; i++) {
    bucket_t *n = array[i].next, *next;

    for(; n; n = next) {
      next = n->next;
      free(n);
    }
  }
}

int
set_rehash(set_t* set, uint32 newlen) {
  bucket_t *array, *ob, *first = NULL, *last = NULL;
  uint32 overflow = 0;

  if(newlen == 0 || (newlen & (newlen - 1)) != 0 || set->array == NULL)
    return 0;

  if((array = calloc(newlen, sizeof(bucket_t))) == NULL)
    return 0;

  /* the values keep their memory, only the buckets are made again, in the order of the set */
  for(ob = set->list; ob; ob = ob->list_next) {
    bucket_t* nb = &array[ob->hash & (newlen - 1)];

    if(nb->value) {
      bucket_t* n;

      if((n = calloc(1, sizeof(bucket_t))) == NULL) {
        free_chains(array, newlen);
        free(array);
        return 0;
      }

      while(nb->next)
        nb = nb->next;

      nb->next = n;
      nb = n;
      ++overflow;
    }

    nb->value = ob->value;
    nb->size = ob->size;
    nb->hash = ob->hash;
    nb->list_prev = last;
    nb->list_next = NULL;

    if(last)
      last->list_next = nb;
    else
      first = nb;

    last = nb;
  }

  free_chains(set->array, set->len);
  free(set->array);

  set->array = array;
  set->len = newlen;
  set->overflow = overflow;
  set->list = first;
  set->last = last;
  return 1;
}
