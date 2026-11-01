#include "../hmap_internal.h"
#include <stdlib.h>

/* Releases the value of an entry (only a copy of chars owns memory). */
int
hmap_free_data(TUPLE* tuple) {
  switch(tuple->data_type) {
    case HMAP_DATA_TYPE_INT: tuple->vals.val_int = 0; break;
    case HMAP_DATA_TYPE_UINT: tuple->vals.val_uint = 0; break;
    case HMAP_DATA_TYPE_INT64: tuple->vals.val_longlong = 0; break;
    case HMAP_DATA_TYPE_UINT64: tuple->vals.val_ulonglong = 0; break;
    case HMAP_DATA_TYPE_DOUBLE: tuple->vals.val_double = 0.0; break;
    case HMAP_DATA_TYPE_CHARS:
      free(tuple->vals.val_chars);
      tuple->vals.val_chars = NULL;
      tuple->data_len = 0;
      break;
    case HMAP_DATA_TYPE_CUSTOM: tuple->vals.val_custom = NULL; break;
    default: return HMAP_DATA_TYPE_INVALID;
  }

  return HMAP_SUCCESS;
}
