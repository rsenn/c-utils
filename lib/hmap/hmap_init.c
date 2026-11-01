#include "../hmap_internal.h"
#include <stdlib.h>

/* Makes an empty map. bucket_size is the size the table starts with (it
 * grows by itself), the table is only made when the first entry comes, so an
 * empty map costs next to nothing. */
int
hmap_init(int bucket_size, HMAP_DB** hmap_db) {
  HMAP_DB* db = (HMAP_DB*)calloc(1, sizeof(HMAP_DB));

  if(db == NULL)
    return HMAP_CREATE_DB_ERROR;

  db->bucket_size = bucket_size > 0 ? bucket_size : 8;

  *hmap_db = db;
  return HMAP_SUCCESS;
}
