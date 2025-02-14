#include "../alloc.h"
#include "../hmap_internal.h"

TUPLE*
hmap_set(HMAP_DB** hmap_db,
         const void* key,
         size_t k_len,
         void* data,
         size_t d_len) {
  TUPLE* t = 0;

  if(!VALID_DB(hmap_db, key, k_len))
    return 0;

  if(hmap_search(*hmap_db, key, k_len, &t) == HMAP_SUCCESS) {
    hmap_free_data(t);
    t->vals.val_chars = (char*)alloc_zero(d_len);
    if(t->vals.val_chars == NULL)
      return 0;
    byte_copy(t->vals.val_chars, d_len, data);
    t->data_len = d_len;
    return t;
  }

  hmap_add(hmap_db, key, k_len, 0, HMAP_DATA_TYPE_CHARS, data, d_len);

  if(hmap_search(*hmap_db, key, k_len, &t) == HMAP_SUCCESS)
    return t;

  return 0;
}
