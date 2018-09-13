#include "../byte.h"
#include "../hmap_internal.h"
#include "hmap_internal.c"
#include <string.h>

int
hmap_add(HMAP_DB** hmap_db, void* key, size_t k_len, int dup_flag, int data_type, ...) {
  
  TUPLE *new_tuple = NULL, *ptr_tuple = NULL, *root_tuple = NULL;
  int r = 0;
  size_t d_len = 0;
  char* data = NULL;
  va_list args;

  int index = hash(key, k_len) % (*hmap_db)->bucket_size;

VALIDATE_DB(*hmap_db, key, k_len);

  va_start(args, data_type);
  r = hmap_search(*hmap_db, key, k_len, &ptr_tuple);

  if(r == HMAP_SUCCESS) {
    return HMAP_TUPLE_ALREADY_EXIST;
  }

  if(r == HMAP_TUPLE_NOT_FOUND) { /* there are avialable record to store */

    root_tuple = ((*hmap_db)->tuple + index);

    if(root_tuple == NULL) {
      return HMAP_TUPUL_EMPTY;
    }

    if(root_tuple->key_len == 0) { /* Reuse record */
      new_tuple = root_tuple;

      HMAP_DUMP("Primary :> ");
      HMAP_SET_DATA();

      byte_copy(new_tuple->key, k_len, key);
      new_tuple->key[k_len] = 0;
      new_tuple->key_len = k_len;
      new_tuple->index = index;
      new_tuple->type = HMAP_TUPLE_PRIMARY;
      new_tuple->data_type = data_type;
      if(root_tuple->hash_next == NULL && root_tuple->hash_prev == NULL) {
        root_tuple->hash_next = root_tuple->hash_prev = root_tuple;
      } else {
        HDB_HASH_APPEND(root_tuple, root_tuple);
      }
      HDB_LIST_APPEND((*hmap_db)->list_tuple, root_tuple);

    } else {
      new_tuple = (TUPLE*)calloc(1, sizeof(TUPLE)); /* Create new recordeto store data */

      if(new_tuple == NULL) {
        return HMAP_ALLOCATED_ERROR;
      }
      HMAP_DUMP("Secondary :> ");
      HMAP_SET_DATA();

      byte_copy(new_tuple->key, k_len, key);
      new_tuple->key[k_len] = 0;
      new_tuple->key_len = k_len;
      new_tuple->index = index;
      new_tuple->hash_next = new_tuple->hash_prev = NULL;
      new_tuple->next = new_tuple->prev = NULL;
      new_tuple->type = HMAP_TUPLE_SECONDARY;
      new_tuple->data_type = data_type;

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
