#include "../hmap_internal.h"

int
hmap_free_data(TUPLE* tuple) {
  switch(tuple->data_type) {
    case HMAP_DATA_TYPE_INT:
      HMAP_DUMP("remove[%d] data[%d]\n", tuple->index, tuple->vals.val_int);
      tuple->vals.val_int = 0;
      break;
    case HMAP_DATA_TYPE_UINT:
      HMAP_DUMP("remove[%d] data[%u]\n", tuple->index, tuple->vals.val_uint);
      tuple->vals.val_uint = 0;
      break;
    case HMAP_DATA_TYPE_INT64:
      HMAP_DUMP("remove[%d] data[%ld]\n",
                tuple->index,
                tuple->vals.val_longlong);
      tuple->vals.val_longlong = 0;
      break;
    case HMAP_DATA_TYPE_UINT64:
      HMAP_DUMP("remove[%d] data[%lu]\n",
                tuple->index,
                tuple->vals.val_ulonglong);
      tuple->vals.val_ulonglong = 0;
      break;
    case HMAP_DATA_TYPE_DOUBLE:
      HMAP_DUMP("remove[%d] data[%f]\n", tuple->index, tuple->vals.val_double);
      tuple->vals.val_double = 0.0;
      break;
    case HMAP_DATA_TYPE_CHARS:
      HMAP_DUMP("remove[%d] data[%s]\n",
                tuple->index,
                (char*)tuple->vals.val_chars);
      if(tuple->vals.val_chars != NULL) {
        free(tuple->vals.val_chars);
      }
      tuple->vals.val_chars = NULL;
      break;
    case HMAP_DATA_TYPE_CUSTOM:
      HMAP_DUMP("remove[%d] data[%p]\n", tuple->index, tuple->vals.val_custom);
      tuple->vals.val_custom = NULL;
      break;
    default: return HMAP_DATA_TYPE_INVALID;
  }
  return HMAP_SUCCESS;
}
