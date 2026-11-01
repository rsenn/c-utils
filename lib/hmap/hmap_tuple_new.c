#include "../hmap_internal.h"
#include <stdlib.h>

TUPLE*
hmap_tuple_new(const void* key, size_t k_len, uint32 hash) {
  TUPLE* t;

  if(k_len + 1 == 0 || (t = (TUPLE*)calloc(1, sizeof(TUPLE) + k_len + 1)) == NULL)
    return NULL;

  /* the key follows the entry in the same piece of memory */
  t->key = (char*)(t + 1);
  byte_copy(t->key, k_len, key);
  t->key[k_len] = '\0';
  t->key_len = k_len;
  t->hash = hash;
  return t;
}
