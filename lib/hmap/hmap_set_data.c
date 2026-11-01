#include "../hmap_internal.h"
#include <stdlib.h>

int
hmap_set_data(TUPLE* t, int data_type, va_list* args) {
  switch(data_type) {
    case HMAP_DATA_TYPE_INT: t->vals.val_int = va_arg(*args, int); break;
    case HMAP_DATA_TYPE_UINT: t->vals.val_uint = va_arg(*args, unsigned int); break;
    case HMAP_DATA_TYPE_INT64: t->vals.val_longlong = va_arg(*args, long int); break;
    case HMAP_DATA_TYPE_UINT64: t->vals.val_ulonglong = va_arg(*args, unsigned long int); break;
    case HMAP_DATA_TYPE_DOUBLE: t->vals.val_double = va_arg(*args, double); break;
    case HMAP_DATA_TYPE_CHARS: {
      const char* data = va_arg(*args, char*);
      size_t d_len = va_arg(*args, size_t);

      if(d_len + 1 == 0 || (t->vals.val_chars = (char*)calloc(1, d_len + 1)) == NULL)
        return HMAP_ALLOCATED_ERROR;

      byte_copy(t->vals.val_chars, d_len, data);
      t->data_len = d_len;
      break;
    }
    case HMAP_DATA_TYPE_CUSTOM: t->vals.val_custom = va_arg(*args, void*); break;
    default: return HMAP_DATA_TYPE_INVALID;
  }

  t->data_type = data_type;
  return HMAP_SUCCESS;
}
