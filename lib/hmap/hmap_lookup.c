#include "../hmap_internal.h"

TUPLE*
hmap_lookup(const HMAP_DB* db, const void* key, size_t k_len, uint32 hash) {
  TUPLE* t;

  if(db->table == NULL)
    return NULL;

  for(t = db->table[hash % (uint32)db->bucket_size]; t; t = t->hash_next)
    if(t->hash == hash && t->key_len == k_len && byte_diff(t->key, k_len, key) == 0)
      return t;

  return NULL;
}
