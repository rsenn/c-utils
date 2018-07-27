#include "hmap_internal.h"

static void hmap_print_list_tuple( HMAP_DB *my_hmap_db ) {
    TUPLE *ptr_list_tuple = my_hmap_db->list_tuple;
    while( ptr_list_tuple ) {
        switch( ptr_list_tuple->data_type ) {
          case HMAP_DATA_TYPE_INT:
            printf("index[%d][%p] key[%s], data[%d]\n", ptr_list_tuple->index, ptr_list_tuple,  ptr_list_tuple->key, ptr_list_tuple->vals.val_int);
            break;
          case HMAP_DATA_TYPE_UINT:
            printf("index[%d][%p] key[%s], data[%u]\n", ptr_list_tuple->index, ptr_list_tuple,  ptr_list_tuple->key, ptr_list_tuple->vals.val_uint);
            break;
          case HMAP_DATA_TYPE_INT64:
            printf("index[%d][%p] key[%s], data[%ld]\n", ptr_list_tuple->index, ptr_list_tuple,  ptr_list_tuple->key, ptr_list_tuple->vals.val_int64);
            break;
          case HMAP_DATA_TYPE_UNIT64:
            printf("index[%d][%p] key[%s], data[%lu]\n", ptr_list_tuple->index, ptr_list_tuple,  ptr_list_tuple->key, ptr_list_tuple->vals.val_uint64);
            break;
          case HMAP_DATA_TYPE_DOUBLE:
            printf("index[%d][%p] key[%s], data[%f]\n", ptr_list_tuple->index, ptr_list_tuple,  ptr_list_tuple->key, ptr_list_tuple->vals.val_double);
            break;
          case HMAP_DATA_TYPE_CHARS:
            printf("index[%d][%p] key[%s], data[%s]\n", ptr_list_tuple->index, ptr_list_tuple,  ptr_list_tuple->key, ptr_list_tuple->vals.val_chars);
            break;
          case HMAP_DATA_TYPE_CUSTOM:
            printf("index[%d][%p] key[%s], data[%p]\n", ptr_list_tuple->index, ptr_list_tuple,  ptr_list_tuple->key, ptr_list_tuple->vals.val_custom);
            break;
        }
        if(ptr_list_tuple->next == my_hmap_db->list_tuple) {
            break;
        }
        ptr_list_tuple = ptr_list_tuple->next;
    }
}

int hmap_print_list( HMAP_DB *my_hmap_db ) {
    if( my_hmap_db == NULL ) 
        return HMAP_DB_EMPTY;
    hmap_print_list_tuple(my_hmap_db);
    return HMAP_SUCCESS;
}
