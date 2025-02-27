#include "../hmap_internal.h"

int
hmap_init(int bucket_size, HMAP_DB** hmap_db) {
  HMAP_DB* new_hmap_db = (HMAP_DB*)calloc(1, sizeof(HMAP_DB));

  if(new_hmap_db == NULL) {
    return HMAP_CREATE_DB_ERROR;
  }

  new_hmap_db->bucket_size = bucket_size;
  new_hmap_db->tuple_count = 0;
  new_hmap_db->list_tuple = NULL;
  new_hmap_db->tuple = (TUPLE*)calloc(bucket_size, sizeof(TUPLE));

  if(new_hmap_db->tuple == NULL) {
    return HMAP_CREATE_TUPUL_ERROR;
  }

  *hmap_db = new_hmap_db;

  return HMAP_SUCCESS;
}
