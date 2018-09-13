#include "../byte.h"

#define HMAP_SET_DATA() \
  { \
    switch(data_type) { \
      case HMAP_DATA_TYPE_INT: \
        new_tuple->vals.val_int = va_arg(args, int); \
        HMAP_DUMP("add[%d] data[%d]\n", index, new_tuple->vals.val_int); \
        break; \
      case HMAP_DATA_TYPE_UINT: \
        new_tuple->vals.val_uint = va_arg(args, unsigned int); \
        HMAP_DUMP("add[%d] data[%u]\n", index, new_tuple->vals.val_uint); \
        break; \
      case HMAP_DATA_TYPE_INT64: \
        new_tuple->vals.val_int64 = va_arg(args, long int); \
        HMAP_DUMP("add[%d] data[%ld]\n", index, new_tuple->vals.val_int64); \
        break; \
      case HMAP_DATA_TYPE_UINT64: \
        new_tuple->vals.val_uint64 = va_arg(args, unsigned long int); \
        HMAP_DUMP("add[%d] data[%lu]\n", index, new_tuple->vals.val_uint64); \
        break; \
      case HMAP_DATA_TYPE_DOUBLE: \
        new_tuple->vals.val_double = va_arg(args, double); \
        HMAP_DUMP("add[%d] data[%f]\n", index, new_tuple->vals.val_double); \
        break; \
      case HMAP_DATA_TYPE_CHARS: \
        data = va_arg(args, char*); \
        d_len = va_arg(args, size_t); \
        new_tuple->vals.val_chars = (char*)calloc(1, d_len + 1); \
        if(new_tuple->vals.val_chars == NULL) { \
          free(new_tuple); \
          return HMAP_ALLOCATED_ERROR; \
        } \
        byte_copy(new_tuple->vals.val_chars, d_len, data); \
        \
        ((char*)new_tuple->vals.val_chars)[d_len] = 0; \
        new_tuple->data_len = d_len; \
        HMAP_DUMP("add[%d] data[%s]\n", index, (char*)new_tuple->vals.val_chars); \
        break; \
      case HMAP_DATA_TYPE_CUSTOM: \
        new_tuple->vals.val_custom = va_arg(args, void*); \
        HMAP_DUMP("add[%d] data[%p]\n", index, new_tuple->vals.val_custom); \
        break; \
      default: return HMAP_DATA_TYPE_INVALID; \
    } \
  }

//inline int
//hmap_cmp(void* v1, int v1_len, void* v2, int v2_len) {
//}

//inline int
//hash(void* key, unsigned int len) {

//}
