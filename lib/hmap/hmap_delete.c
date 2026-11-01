#include "../hmap_internal.h"
#include <stdlib.h>

/* Takes the entry out and frees it: HMAP_SUCCESS, or HMAP_TUPLE_NOT_FOUND. */
int
hmap_delete(HMAP_DB** hmap_db, const void* key, size_t k_len) {
  TUPLE **link, *t;
  uint32 hash;

  VALIDATE_DB(*hmap_db, key, k_len);

  if((*hmap_db)->table == NULL)
    return HMAP_TUPLE_NOT_FOUND;

  hash = hmap_hash(key, k_len);

  for(link = &(*hmap_db)->table[hash % (uint32)(*hmap_db)->bucket_size]; (t = *link); link = &t->hash_next)
    if(t->hash == hash && t->key_len == k_len && byte_diff(t->key, k_len, key) == 0)
      break;

  if(t == NULL)
    return HMAP_TUPLE_NOT_FOUND;

  *link = t->hash_next;
  HDB_LIST_REMOVE((*hmap_db)->list_tuple, t);
  hmap_free_data(t);
  free(t);
  (*hmap_db)->tuple_count--;

  return HMAP_SUCCESS;
}
