#include "../hmap_internal.h"
#include <stdlib.h>

/* the table gets twice the buckets; the entries stay where they are in memory */
static void
grow(HMAP_DB* db) {
  TUPLE **table, *t;
  uint32 size = (uint32)db->bucket_size * 2;

  if((int)size <= 0 || (table = (TUPLE**)calloc(size, sizeof(TUPLE*))) == NULL)
    return; /* the chains get longer, nothing else */

  if((t = db->list_tuple)) {
    do {
      uint32 i = t->hash % size;

      t->hash_next = table[i];
      table[i] = t;
      t = t->next;
    } while(t != db->list_tuple);
  }

  free(db->table);
  db->table = table;
  db->bucket_size = (int)size;
}

int
hmap_link(HMAP_DB* db, TUPLE* t) {
  int index;

  if(db->table == NULL) {
    if((db->table = (TUPLE**)calloc((size_t)db->bucket_size, sizeof(TUPLE*))) == NULL)
      return HMAP_ALLOCATED_ERROR;
  } else if(db->tuple_count >= db->bucket_size) {
    grow(db);
  }

  index = (int)(t->hash % (uint32)db->bucket_size);

  t->hash_next = db->table[index];
  db->table[index] = t;

  HDB_LIST_APPEND(db->list_tuple, t);
  db->tuple_count++;

  return index;
}
