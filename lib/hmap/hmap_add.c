#include "../hmap_internal.h"
#include <stdlib.h>

/* Adds an entry; the value that follows data_type is an int, an unsigned int,
 * a long, an unsigned long or a double for the numeric types, a char* and its
 * size_t length for HMAP_DATA_TYPE_CHARS (copied, with a NUL added), a void*
 * for HMAP_DATA_TYPE_CUSTOM (kept as it is). Returns the number of the bucket
 * (0 or more), HMAP_TUPLE_ALREADY_EXIST if the key is in the map, or an error
 * code. */
int
hmap_add(HMAP_DB** hmap_db, const void* key, size_t k_len, int dup_flag, int data_type, ...) {
  TUPLE* t;
  uint32 hash;
  va_list args;
  int r;

  (void)dup_flag;

  VALIDATE_DB(*hmap_db, key, k_len);

  hash = hmap_hash(key, k_len);

  if(hmap_lookup(*hmap_db, key, k_len, hash))
    return HMAP_TUPLE_ALREADY_EXIST;

  if((t = hmap_tuple_new(key, k_len, hash)) == NULL)
    return HMAP_ALLOCATED_ERROR;

  va_start(args, data_type);
  r = hmap_set_data(t, data_type, &args);
  va_end(args);

  if(r != HMAP_SUCCESS) {
    free(t);
    return r;
  }

  if((r = hmap_link(*hmap_db, t)) < 0) {
    hmap_free_data(t);
    free(t);
  }

  return r;
}
