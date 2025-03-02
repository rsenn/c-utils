#include "../byte.h"
#include "../hmap_internal.h"

int
hmap_add_tuple_with_data(HMAP_DB** hmap_db, const void* key, size_t k_len, void* data) {
  TUPLE *new_tuple = NULL, *ptr_tuple = NULL, *root_tuple = NULL;
  int r = 0;
  int index = hmap_hash(key, k_len) % (*hmap_db)->bucket_size;

  VALIDATE_DB(*hmap_db, key, k_len);

  r = hmap_search(*hmap_db, key, k_len, &ptr_tuple);

  if(r == HMAP_SUCCESS) {
    return HMAP_TUPLE_ALREADY_EXIST;
  }

  if(r == HMAP_TUPLE_NOT_FOUND) {
    root_tuple = ((*hmap_db)->tuple + index);

    if(root_tuple == NULL) {
      return HMAP_TUPUL_EMPTY;
    }
    /* free slot */

    if(root_tuple->key_len == 0) {
      root_tuple->data = data;
      root_tuple->data_len = -1;

      HMAP_DUMP("add[%d] primary data[%s]\n", index, (char*)root_tuple->data);
      byte_copy(root_tuple->key, k_len, key);
      root_tuple->key[k_len] = 0;
      root_tuple->key_len = k_len;
      root_tuple->index = index;
      root_tuple->type = HMAP_TUPLE_PRIMARY;

      if(root_tuple->hash_next == NULL && root_tuple->hash_prev == NULL) {
        root_tuple->hash_next = root_tuple->hash_prev = root_tuple;
      } else {
        HDB_HASH_APPEND(root_tuple, root_tuple);
      }
      HDB_LIST_APPEND((*hmap_db)->list_tuple, root_tuple);

    } else {
      new_tuple = (TUPLE*)calloc(1, sizeof(TUPLE));

      if(new_tuple == NULL) {
        return HMAP_ALLOCATED_ERROR;
      }

      new_tuple->data = data;
      new_tuple->data_len = -1;

      HMAP_DUMP("add[%d] secondary data[%s]\n", index, (char*)new_tuple->data);
      byte_copy(new_tuple->key, k_len, key);
      new_tuple->key[k_len] = 0;
      new_tuple->key_len = k_len;
      new_tuple->index = index;
      new_tuple->hash_next = new_tuple->hash_prev = NULL;
      new_tuple->next = new_tuple->prev = NULL;
      new_tuple->type = HMAP_TUPLE_SECONDARY;
      (*hmap_db)->secondary_tuple_count++;
      HDB_LIST_APPEND((*hmap_db)->list_tuple, new_tuple);
      HDB_HASH_APPEND(root_tuple, new_tuple);
    }

  } else {
    return r;
  }

  (*hmap_db)->tuple_count++;

  return index;
}
