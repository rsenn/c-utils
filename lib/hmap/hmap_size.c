#include "../hmap_internal.h"

/* Number of entries. */
size_t
hmap_size(HMAP_DB* my_hmap_db) {
  return my_hmap_db ? (size_t)my_hmap_db->tuple_count : 0;
}
