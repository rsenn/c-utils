#include "../hmap_internal.h"

static void hmap_print( HMAP_DB *my_hmap_db ) {
  int i = 0;
  TUPLE * my_tuple = NULL;
  my_tuple = my_hmap_db->tuple;
  for(i = 0; i < my_hmap_db->bucket_size; i++) {
      switch( my_tuple->data_type ) {
          case HMAP_DATA_TYPE_INT:
            printf("index[%d][%p] key[%s], data[%d]\n", i, my_tuple , my_tuple->key, my_tuple->vals.val_int);
            break;
          case HMAP_DATA_TYPE_UINT:
            printf("index[%d][%p] key[%s], data[%u]\n", i, my_tuple , my_tuple->key, my_tuple->vals.val_uint);
            break;
          case HMAP_DATA_TYPE_INT64:
            printf("index[%d][%p] key[%s], data[%ld]\n", i, my_tuple , my_tuple->key, my_tuple->vals.val_int64);
            break;
          case HMAP_DATA_TYPE_UNIT64:
            printf("index[%d][%p] key[%s], data[%lu]\n", i, my_tuple , my_tuple->key, my_tuple->vals.val_uint64);
            break;
          case HMAP_DATA_TYPE_DOUBLE:
            printf("index[%d][%p] key[%s], data[%f]\n", i, my_tuple , my_tuple->key, my_tuple->vals.val_double);
            break;
          case HMAP_DATA_TYPE_CHARS:
            printf("index[%d][%p] key[%s], data[%s]\n", i, my_tuple , my_tuple->key, my_tuple->vals.val_chars);
            break;
          case HMAP_DATA_TYPE_CUSTOM:
            printf("index[%d][%p] key[%s], data[%p]\n", i, my_tuple , my_tuple->key, my_tuple->vals.val_custom);
            break;
        }
        my_tuple++;
    }
}

int hmap_print_table( HMAP_DB *my_hmap_db ) {
    if( my_hmap_db == NULL )
        return HMAP_DB_EMPTY;
    hmap_print(my_hmap_db);
    return HMAP_SUCCESS;
}
