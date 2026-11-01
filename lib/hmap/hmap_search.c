#include "../hmap_internal.h"

/* HMAP_SUCCESS and the entry in *data (if data is not 0), or HMAP_TUPLE_NOT_FOUND */
int
hmap_search(HMAP_DB* hmap_db, const void* key, size_t k_len, TUPLE** data) {
  TUPLE* t;

  VALIDATE_DB(hmap_db, key, k_len);

  if((t = hmap_lookup(hmap_db, key, k_len, hmap_hash(key, k_len))) == NULL)
    return HMAP_TUPLE_NOT_FOUND;

  if(data)
    *data = t;

  return HMAP_SUCCESS;
}
