#include "../hmap_internal.h"

int
hmap_truncate(HMAP_DB** hmap_db) {
  TUPLE* ptr_list_tuple = (*hmap_db)->list_tuple;
  while(ptr_list_tuple) {
    HDB_LIST_REMOVE((*hmap_db)->list_tuple, ptr_list_tuple);
    if(hmap_free_data(ptr_list_tuple) != HMAP_SUCCESS) {
      HMAP_DUMP("Warnning: cannot free data\n", 0, 0);
    }
    if(ptr_list_tuple->data) {
      free(ptr_list_tuple->data);
      ptr_list_tuple->data = NULL;
      ptr_list_tuple->key[0] = 0;
      ptr_list_tuple->key_len = 0;
    }
    if(ptr_list_tuple->type == HMAP_TUPLE_SECONDARY) {
      free(ptr_list_tuple);
    }
    ptr_list_tuple = (*hmap_db)->list_tuple;
  }
  return 0;
}
