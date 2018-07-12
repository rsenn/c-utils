#include "../hmap_internal.h"

static void
hmap_print_tree_tuple(HMAP_DB* my_hmap_db) {
  int i = 0;
  TUPLE *root_tuple, *ptr_list_tuple;
  for(; i < my_hmap_db->bucket_size; i++) {
    ptr_list_tuple = root_tuple = (my_hmap_db->tuple + i);
    if(root_tuple == NULL) {
      continue;
    }
    printf("index[%d]\n", ptr_list_tuple->index);
    while(ptr_list_tuple) {
      if(ptr_list_tuple->key_len > 0) {
        switch(ptr_list_tuple->data_type) {
          case HMAP_DATA_TYPE_INT:
            printf("index[%d][%p] key[%s], data[%d]\n", ptr_list_tuple->index, ptr_list_tuple, ptr_list_tuple->key, ptr_list_tuple->vals.val_int);
            break;
          case HMAP_DATA_TYPE_UINT:
            printf("index[%d][%p] key[%s], data[%u]\n", ptr_list_tuple->index, ptr_list_tuple, ptr_list_tuple->key, ptr_list_tuple->vals.val_uint);
            break;
          case HMAP_DATA_TYPE_INT64:
            printf("index[%d][%p] key[%s], data[%ld]\n", ptr_list_tuple->index, ptr_list_tuple, ptr_list_tuple->key, ptr_list_tuple->vals.val_int64);
            break;
          case HMAP_DATA_TYPE_UNIT64:
            printf("index[%d][%p] key[%s], data[%lu]\n", ptr_list_tuple->index, ptr_list_tuple, ptr_list_tuple->key, ptr_list_tuple->vals.val_uint64);
            break;
          case HMAP_DATA_TYPE_DOUBLE:
            printf("index[%d][%p] key[%s], data[%f]\n", ptr_list_tuple->index, ptr_list_tuple, ptr_list_tuple->key, ptr_list_tuple->vals.val_double);
            break;
          case HMAP_DATA_TYPE_CHARS:
            printf("index[%d][%p] key[%s], data[%s]\n", ptr_list_tuple->index, ptr_list_tuple, ptr_list_tuple->key, ptr_list_tuple->vals.val_chars);
            break;
          case HMAP_DATA_TYPE_CUSTOM:
            printf("index[%d][%p] key[%s], data[%p]\n", ptr_list_tuple->index, ptr_list_tuple, ptr_list_tuple->key, ptr_list_tuple->vals.val_custom);
            break;
        }
      }
      if(ptr_list_tuple->hash_next == root_tuple) {
        break;
      }
      ptr_list_tuple = ptr_list_tuple->hash_next;
    }
  }
}

int
hmap_print_tree(HMAP_DB* my_hmap_db) {
  if(my_hmap_db == NULL) return HMAP_DB_EMPTY;
  hmap_print_tree_tuple(my_hmap_db);
  return HMAP_SUCCESS;
}
