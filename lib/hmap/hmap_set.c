#include "../hmap_internal.h"

int hmap_set(HMAP_DB** hmap_db, void* key, size_t k_len, void* data, size_t d_len) {
  return hmap_add(hmap_db, key, k_len, 1, HMAP_DATA_TYPE_CHARS, data, d_len);
 }
