#include "../hmap_internal.h"
#include <stdlib.h>

/* Sets the value of the key to a copy of the d_len bytes at data (and a NUL
 * after them), adding the entry if the key is new. Returns the entry, or 0 if
 * the memory ran out. */
TUPLE*
hmap_set(HMAP_DB** hmap_db, const void* key, size_t k_len, void* data, size_t d_len) {
  TUPLE* t = 0;
  char* copy;

  if(!VALID_DB(hmap_db, key, k_len))
    return 0;

  if(hmap_search(*hmap_db, key, k_len, &t) != HMAP_SUCCESS) {
    if(hmap_add(hmap_db, key, k_len, 0, HMAP_DATA_TYPE_CHARS, data, d_len) < 0)
      return 0;

    return hmap_search(*hmap_db, key, k_len, &t) == HMAP_SUCCESS ? t : 0;
  }

  if(d_len + 1 == 0 || (copy = (char*)calloc(1, d_len + 1)) == NULL)
    return 0;

  byte_copy(copy, d_len, data);

  hmap_free_data(t);
  t->vals.val_chars = copy;
  t->data_len = d_len;
  t->data_type = HMAP_DATA_TYPE_CHARS;
  return t;
}
