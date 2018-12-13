#include "../hmap.h"

void*
hmap_get(HMAP_DB* db, const char* key, size_t keylen) {
  TUPLE* ptr_tuple;
  if(hmap_search(db, key, keylen, &ptr_tuple) == HMAP_SUCCESS)
    return ptr_tuple->vals.val_chars;
  return NULL;
}