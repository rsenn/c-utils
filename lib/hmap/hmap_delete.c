#include "../hmap_internal.h"

int
hmap_delete(HMAP_DB** hmap_db, void* key, int k_len) {

  VALIDATE_DB(*hmap_db, key, k_len);

  TUPLE *root_tuple = NULL, *ptr_tuple = NULL;
  int r = hmap_search(*hmap_db, key, k_len, &ptr_tuple);

  if(r == HMAP_SUCCESS) {
    HDB_LIST_REMOVE((*hmap_db)->list_tuple, ptr_tuple);
    root_tuple = (*hmap_db)->tuple + ptr_tuple->index;
    if(root_tuple == ptr_tuple) {
      root_tuple->key[0] = 0;
      root_tuple->key_len = 0;
      if(hmap_free_data(ptr_tuple) != HMAP_SUCCESS) {
        HMAP_DEBUG("Warnning: cannot free data\n");
      }
      if(root_tuple->data != NULL) {
        free(root_tuple->data);
      }
      root_tuple->data = NULL;
      root_tuple->data_len = 0;
    } else {
      HDB_HASH_REMOVE(root_tuple, ptr_tuple);
      if(hmap_free_data(ptr_tuple) != HMAP_SUCCESS) {
        HMAP_DEBUG("Warnning: cannot free data\n");
      }
      if(ptr_tuple->data != NULL) {
        free(ptr_tuple->data);
      }
      free(ptr_tuple);
      (*hmap_db)->secondary_tuple_count--;
    }
    (*hmap_db)->tuple_count--;
  }

  return r;
}
