#include "uint32.h"
#include "byte.h"
#include "hmap.h"
#include <stdarg.h>

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

/* the circular list of entries in the order of insertion */
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

/* hash of a key */
uint32 hmap_hash(const void* key, size_t len);

/* the entry for the key, or NULL; hash is hmap_hash of the key */
TUPLE* hmap_lookup(const HMAP_DB* db, const void* key, size_t k_len, uint32 hash);

/* a new entry for the key with no data yet, not in any map; NULL if the memory ran out */
TUPLE* hmap_tuple_new(const void* key, size_t k_len, uint32 hash);

/* puts the entry in the map (grows the table first if needed): the bucket number, or HMAP_ALLOCATED_ERROR */
int hmap_link(HMAP_DB* db, TUPLE* t);

/* sets the value of a new entry from the arguments of hmap_add; 0 or an error code */
int hmap_set_data(TUPLE* t, int data_type, va_list* args);
