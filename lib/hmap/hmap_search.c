#include <stdarg.h>
#include <string.h>
#include "../hmap_internal.h"
#include "hmap_internal.c"

int hmap_search(HMAP_DB *hmap_db, void *key, int k_len, TUPLE **data) {

    VALIDATE_DB(hmap_db, key, k_len);

    int index = hash(key, k_len)%hmap_db->bucket_size;
    TUPLE *ptr_tuple = (hmap_db->tuple+index);
    TUPLE *ptr_hash_tuple = NULL;

    if(ptr_tuple == NULL) {
        return HMAP_TUPUL_EMPTY;
    }

    TUPLE *itr_tuple = ptr_tuple;
    while( itr_tuple ) {
        ptr_hash_tuple = itr_tuple;
        while( ptr_hash_tuple ) {

            if( hmap_cmp(ptr_hash_tuple->key, ptr_hash_tuple->key_len, key, k_len) == 0) {
                if(data != NULL) {
                    *data = ptr_hash_tuple;
                }
                return HMAP_SUCCESS;
            }

            if(ptr_hash_tuple->hash_next == itr_tuple ) {
                break;
            }
            ptr_hash_tuple = ptr_hash_tuple->hash_next;
        }

        if(itr_tuple->hash_next == ptr_tuple ) {
            break;
        }
        itr_tuple = itr_tuple->hash_next;
    }

    return HMAP_TUPLE_NOT_FOUND;
}
