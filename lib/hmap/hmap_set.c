#include "../hmap_internal.h"

int
hmap_set(HMAP_DB** hmap_db, const void* key, size_t k_len, void* data, size_t d_len) {
  TUPLE* t;

  VALIDATE_DB(hmap_db, key, k_len);

  if(hmap_search(*hmap_db, key, k_len, &t) == HMAP_SUCCESS) {

    hmap_free_data(t);

    t->vals.val_chars = calloc(1, d_len);

    if(t->vals.val_chars == NULL)
      return HMAP_ALLOCATED_ERROR;

    byte_copy(t->vals.val_chars, d_len, data);
    t->data_len = d_len;

    return HMAP_SUCCESS;
  }
  return hmap_add(hmap_db, key, k_len, 1, HMAP_DATA_TYPE_CHARS, data, d_len);
}
