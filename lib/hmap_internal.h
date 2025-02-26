#include "uint32.h"
#include "byte.h"
#include "hmap.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

//#define HMAP_DUMP(...) do {printf( __VA_ARGS__ );}while(0)

#if defined(HMAP_DEBUG) && !defined(NDEBUG)
#include "buffer.h"
#ifdef __BORLANDC__
#define HMAP_DUMP(a1, a2, a3) \
  do { \
    fprintf(stderr, a1, a2, a3); \
    fflush(stderr); \
  } while(0)
#else
#define HMAP_DUMP(...) \
  do { \
    fprintf(stderr, __VA_ARGS__); \
    fflush(stderr); \
  } while(0)
#endif
#else
#define HMAP_DUMP(a1, a2, a3)
#endif
#define VALID_DB(_hdb, _key, _klen) ((_hdb) && (_key) && (_klen > 0))

#define VALIDATE_DB(_hdb, _key, _klen) \
  do { \
    if(_hdb == NULL) { \
      return HMAP_DB_EMPTY; \
    } \
    if(_key == NULL || _klen <= 0) { \
      return HMAP_KEY_EMPTY; \
    } \
  } while(0)

#define HDB_LIST_APPEND(_first, _item) \
  { \
    if((_first) == NULL) { \
      (_first) = (_item)->prev = (_item)->next = (_item); \
    } else { \
      (_item)->prev = (_first)->prev; \
      (_item)->next = (_first); \
      (_first)->prev->next = (_item); \
      (_first)->prev = (_item); \
    } \
  }
#define HDB_LIST_REMOVE(_first, _item) \
  { \
    if((_first) == (_item)) { \
      if((_first)->next == (_first)) \
        (_first) = NULL; \
      else { \
        (_first) = (_item)->next; \
        (_item)->next->prev = (_item)->prev; \
        (_item)->prev->next = (_item)->next; \
      } \
    } else { \
      (_item)->next->prev = (_item)->prev; \
      (_item)->prev->next = (_item)->next; \
    } \
    (_item)->prev = (_item)->next = NULL; \
  }

#define HDB_HASH_APPEND(_first, _item) \
  { \
    if((_first) == NULL) { \
      (_first) = (_item)->hash_prev = (_item)->hash_next = (_item); \
    } else { \
      (_item)->hash_prev = (_first)->hash_prev; \
      (_item)->hash_next = (_first); \
      (_first)->hash_prev->hash_next = (_item); \
      (_first)->hash_prev = (_item); \
    } \
  }

#define HDB_HASH_REMOVE(_first, _item) \
  { \
    if((_first) == (_item)) { \
      if((_first)->hash_next == (_first)) \
        (_first) = NULL; \
      else { \
        (_first) = (_item)->hash_next; \
        (_item)->hash_next->hash_prev = (_item)->hash_prev; \
        (_item)->hash_prev->hash_next = (_item)->hash_next; \
      } \
    } else { \
      (_item)->hash_next->hash_prev = (_item)->hash_prev; \
      (_item)->hash_prev->hash_next = (_item)->hash_next; \
    } \
    (_item)->hash_prev = (_item)->hash_next = NULL; \
  }

#define IS_NEXT_HASH(_first, _curr) \
  { \
    if(_curr->hash_next == _first) { \
      break; \
    } \
    _curr = _curr->hash_next; \
  }

#define IS_NEXT_LIST(_first, _curr) \
  { \
    if(_curr->next == _first) { \
      break; \
    } \
    _curr = _curr->next; \
  }

int hmap_free_data(TUPLE* tuple);

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
        new_tuple->vals.val_longlong = va_arg(args, long int); \
        HMAP_DUMP("add[%d] data[%ld]\n", index, new_tuple->vals.val_longlong); \
        break; \
      case HMAP_DATA_TYPE_UINT64: \
        new_tuple->vals.val_ulonglong = va_arg(args, unsigned long int); \
        HMAP_DUMP("add[%d] data[%lu]\n", index, new_tuple->vals.val_ulonglong); \
        break; \
      case HMAP_DATA_TYPE_DOUBLE: \
        new_tuple->vals.val_double = va_arg(args, double); \
        HMAP_DUMP("add[%d] data[%f]\n", index, new_tuple->vals.val_double); \
        break; \
      case HMAP_DATA_TYPE_CHARS: \
        data = va_arg(args, char*); \
        d_len = va_arg(args, size_t); \
        new_tuple->vals.val_chars = (char*)calloc(1, d_len + 1); \
        \
        if(new_tuple->vals.val_chars == NULL) { \
          free(new_tuple); \
          return HMAP_ALLOCATED_ERROR; \
        } \
        \
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

int hmap_cmp(void* v1, size_t v1_len, void* v2, size_t v2_len);
int32 hmap_hash(const void* key, size_t len);
