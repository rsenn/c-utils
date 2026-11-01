#include "../hmap_internal.h"
#include <stdlib.h>

/* Removes all entries, the map stays usable. */
int
hmap_truncate(HMAP_DB** hmap_db) {
  HMAP_DB* db = *hmap_db;
  TUPLE *t = db->list_tuple, *next;

  while(t) {
    next = (t->next == db->list_tuple) ? NULL : t->next;
    hmap_free_data(t);
    free(t);
    t = next;
  }

  if(db->table)
    byte_zero(db->table, (size_t)db->bucket_size * sizeof(TUPLE*));

  db->list_tuple = NULL;
  db->tuple_count = 0;
  return 0;
}
