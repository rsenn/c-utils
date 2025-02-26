/**
 * @defgroup   hmap
 * @brief      HMAP module.
 * @{
 */
#ifndef HMAP_H
#define HMAP_H 1

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAP_BUCKET 1024
#define MAX_SIZE_KEY 1024

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

enum hmap_tuple_type { HMAP_TUPLE_PRIMARY = 0, HMAP_TUPLE_SECONDARY };

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

typedef struct tuple {
  struct tuple* next;
  struct tuple* prev;
  struct tuple* hash_next;
  struct tuple* hash_prev;

  char key[MAX_SIZE_KEY];
  size_t key_len;
  int type;
  int index;
  size_t data_len;
  void* data;

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

typedef struct hmap_db {
  int tuple_count;
  int secondary_tuple_count;
  int bucket_size;
  TUPLE* tuple;
  TUPLE* list_tuple;
} HMAP_DB;

int hmap_add_tuple_with_data(HMAP_DB** hmap_db, const void* key, size_t k_len, void* data);
int hmap_add(HMAP_DB** hmap_db, const void* key, size_t k_len, int dup_flag, int data_type, ...);
int hmap_delete(HMAP_DB** hmap_db, const void* key, size_t k_len);
int hmap_destroy(HMAP_DB** hmap_db);
int hmap_free_data(TUPLE* tuple);
int hmap_init(int bucket_size, HMAP_DB** hmap_db);
int hmap_is_locate(HMAP_DB* hmap_db, void* key, size_t k_len);
int hmap_print_list(HMAP_DB* my_hmap_db);
int hmap_print_table(HMAP_DB* my_hmap_db);
int hmap_print_tree(HMAP_DB* my_hmap_db);
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

#define hmap_iterator_distance(it1, it2) hmap_distance(&(it1), &(it2))
int hmap_distance(TUPLE** it1, TUPLE** it2);

#define hmap_iterator_equal(it1, it2) ((it1) == (it2))
/*#define hmap_iterator_equal(it1, it2) ( ((it1) == NULL && (it2)->next ==
  (it2)) || \
  ((it2) == NULL && (it1)->next == (it1)) || \
  hmap_iterator_distance(it1, it2) == 0)
*/
//#define hmap_iterator_is_last(hmap_db,it)  ((hmap_db)->list_tuple ==
//(*(it))->next)

#ifdef STRALLOC_H
int hmap_set_stralloc(HMAP_DB** hmap_db, const stralloc* key, const stralloc* data);
#endif

#ifdef BUFFER_H
void hmap_dump(HMAP_DB* my_hmap_db, buffer*);
#endif

#ifdef __cplusplus
}
#endif
#endif /* defined HMAP_H */
/** @} */
