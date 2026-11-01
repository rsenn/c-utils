#include "../hmap_internal.h"
#include <stdlib.h>

/* Frees the map and all its entries; *hmap_db is NULL afterwards. */
int
hmap_destroy(HMAP_DB** hmap_db) {
  if(*hmap_db == NULL)
    return 0;

  hmap_truncate(hmap_db);
  free((*hmap_db)->table);
  free(*hmap_db);
  *hmap_db = NULL;
  return 0;
}
