#include "../hmap_internal.h"

int
hmap_destroy(HMAP_DB** hmap_db) {
  hmap_truncate(hmap_db);
  free((*hmap_db)->tuple);
  free((*hmap_db));
  *hmap_db = NULL;
  return 0;
}

