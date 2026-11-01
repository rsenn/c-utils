/**
 * @defgroup   hmap
 * @brief      HMAP module.
 * @{
 */
#ifndef HMAP_H
#define HMAP_H 1

#include "typedefs.h"
#include "uint32.h"
#include "stralloc.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif


enum hmap_error_enum {
  HMAP_SUCCESS = 0,
  HMAP_FAILED = -1,
  HMAP_ALLOCATED_ERROR = -100,
  HMAP_CREATE_DB_ERROR = -101,
  HMAP_CREATE_TUPUL_ERROR = -102,
  HMAP_DB_EMPTY = -401,
  HMAP_TUPUL_EMPTY = -403,
  HMAP_KEY_EMPTY = -404,
  HMAP_TUPLE_NOT_FOUND = -405,
  HMAP_TUPLE_DUPLICATE = -406,
  HMAP_TUPLE_ALREADY_EXIST = -407,

  HMAP_DATA_TYPE_INVALID,
  HMAP_ALL
};


typedef enum hmap_data_type {
  HMAP_DATA_TYPE_INT = 0,
  HMAP_DATA_TYPE_UINT,
  HMAP_DATA_TYPE_INT64,
  HMAP_DATA_TYPE_UINT64,
  HMAP_DATA_TYPE_DOUBLE,
  HMAP_DATA_TYPE_CHARS,
  HMAP_DATA_TYPE_CUSTOM,
  HMAP_DATA_TYPE_TUPLE,
} HMAP_DATA_TYPE;

/* One entry of a map. Read its fields, never write them. */
typedef struct tuple {
  struct tuple* next; /* the next entry in the order of insertion; circular */
  struct tuple* prev;
  struct tuple* hash_next; /* the next entry of the same bucket */
  char* key;               /* copy of the key, ends in a NUL byte, in the memory of the entry */
  size_t key_len;
  uint32 hash;
  size_t data_len;
  int data_type;
  union {
    int val_int;
    unsigned int val_uint;
    long int val_longlong;
    unsigned long int val_ulonglong;

    double val_double;
    char* val_chars;
    struct tuple* val_tuple;
    void* val_custom;
  } vals;
} TUPLE;

/* A map: entries are found by hash and kept in the order of insertion. The
 * table is made when the first entry comes and doubles when there are more
 * entries than buckets, so adding, finding and deleting take constant time. */
typedef struct hmap_db {
  int tuple_count;
  int bucket_size;
  TUPLE** table;
  TUPLE* list_tuple;
} HMAP_DB;

int hmap_add(HMAP_DB** hmap_db, const void* key, size_t k_len, int dup_flag, int data_type, ...);
int hmap_delete(HMAP_DB** hmap_db, const void* key, size_t k_len);
int hmap_destroy(HMAP_DB** hmap_db);
int hmap_free_data(TUPLE* tuple);
int hmap_init(int bucket_size, HMAP_DB** hmap_db);
int hmap_search(HMAP_DB* hmap_db, const void* key, size_t k_len, TUPLE** data);
int hmap_set_chars(HMAP_DB** hmap_db, const char* key, const char* data);
TUPLE* hmap_set(HMAP_DB**, const void*, size_t k_len, void* data, size_t d_len);
size_t hmap_size(HMAP_DB* my_hmap_db);
int hmap_truncate(HMAP_DB** hmap_db);
void* hmap_get(HMAP_DB* db, const char* key, size_t keylen);

inline static void*
hmap_data(TUPLE* tuple) {
  return tuple->vals.val_chars;
}
inline static size_t
hmap_count(HMAP_DB* hmap_db) {
  return hmap_db->tuple_count;
}

//#define hmap_first(hmap_db, tuple) ((hmap_db)->list_tuple)
#define hmap_last(hmap_db, tuple) ((hmap_db)->list_tuple == (tuple)->next)
#define hmap_next(hmap_db, tuple) (hmap_last(hmap_db, tuple) ? NULL : (tuple)->next)

#define hmap_foreach(hmap_db, tuple) for(tuple = hmap_begin(hmap_db); tuple; tuple = hmap_next(hmap_db, tuple))

inline static TUPLE*
hmap_begin(HMAP_DB* hmap) {
  return hmap->list_tuple;
}

inline static void
hmap_iterator_increment(TUPLE** t) {
  *t = (*t)->next;
}
inline static int
hmap_iterator_equal(TUPLE** t1, TUPLE** t2) {
  return (*t1) == (*t2);
}
inline static TUPLE*
hmap_iterator_dereference(TUPLE** it) {
  return *it;
}

//#define hmap_end(hmap_db) (&((hmap_db)->list_tuple->prev->next))
//#define hmap_end(hmap_db) ((hmap_db)->list_tuple ?
//&((hmap_db)->list_tuple->prev->next) : NULL)

#define hmap_iterator_increment(it) ((it) = &(*(it))->next)
#define hmap_iterator_decrement(it) ((it) = &(*(it))->prev)
#define hmap_iterator_dereference(it_ptr) (*(it_ptr))


#define hmap_iterator_equal(it1, it2) ((it1) == (it2))
/*#define hmap_iterator_equal(it1, it2) ( ((it1) == NULL && (it2)->next ==
  (it2)) || \
  ((it2) == NULL && (it1)->next == (it1)) || \
  hmap_iterator_distance(it1, it2) == 0)
*/
//#define hmap_iterator_is_last(hmap_db,it)  ((hmap_db)->list_tuple ==
//(*(it))->next)

int hmap_set_stralloc(HMAP_DB** hmap_db, const stralloc* key, const stralloc* data);

void hmap_dump(HMAP_DB* my_hmap_db, buffer*);

#ifdef __cplusplus
}
#endif
#endif /* defined HMAP_H */
/** @} */
