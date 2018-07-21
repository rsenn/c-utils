#ifndef HMAP_H
#define HMAP_H 1

#include <sys/types.h>

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

enum hmap_data_type {
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
    long int val_int64;
    unsigned long int val_uint64;
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

int hmap_init(int bucket_size, HMAP_DB** hmap_db);
int hmap_is_locate(HMAP_DB* hmap_db, void* key, size_t k_len);
int hmap_add(HMAP_DB** hmap_db, void* key, size_t k_len, int dup_flag, int data_type, ...);
int hmap_add_tuple_with_data(HMAP_DB** hmap_db, void* key, size_t k_len, void* data);
int hmap_set(HMAP_DB** hmap_db, void* key, size_t k_len, void* data, size_t d_len);
int hmap_set_chars(HMAP_DB** hmap_db, const char* key, const char* data);
int hmap_print_table(HMAP_DB* hmap_db);
int hmap_print_list(HMAP_DB* hmap_db);
int hmap_print_tree(HMAP_DB* my_hmap_db);
int hmap_search(HMAP_DB* hmap_db, void* key, size_t k_len, TUPLE** data);
int hmap_destroy(HMAP_DB** hmap_db);
int hmap_truncate(HMAP_DB** HMAP_db);

#define hmap_last(hmap_db, it)  ((hmap_db)->list_tuple == (*(it))->next)

static inline TUPLE* hmap_begin(HMAP_DB* hmap) { return hmap->list_tuple; }

static inline void hmap_iterator_increment(HMAP_DB* hmap, TUPLE** t) { *t = (*t)->next; }

//#define hmap_end(hmap_db) (&((hmap_db)->list_tuple->prev->next))
//#define hmap_end(hmap_db) ((hmap_db)->list_tuple ? &((hmap_db)->list_tuple->prev->next) : NULL)



#define hmap_iterator_increment(it) ((it) = &(*(it))->next)
#define hmap_iterator_decrement(it) ((it) = &(*(it))->prev)
#define hmap_iterator_dereference(it_ptr) (*(it_ptr))
static inline int
hmap_iterator_distance(TUPLE** it1, TUPLE** it2) {
  TUPLE *a = *it1;
  TUPLE *b = *it2;
  int n = 0;

  while(a && a != *it2 && a != a->next) {
    a = a->next;
    ++n;
  }
  if(a == *it2 || (!it2 && a == a->next)) return n;

  n = 0;
  while(b && b != *it1 && b != b->next) {
    b = b->next;
    ++n;
  }
  if(b == * it1 || (!it1  && b == b->next)) return n;

  return -1;
}

#define hmap_iterator_equal(it1, it2) ((it1) == (it2))
/*#define hmap_iterator_equal(it1, it2) ( \
  ((it1) == NULL && (it2)->next == (it2)) || \
  ((it2) == NULL && (it1)->next == (it1)) || \
  hmap_iterator_distance(it1, it2) == 0)
*/
//#define hmap_iterator_is_last(hmap_db,it)  ((hmap_db)->list_tuple == (*(it))->next)

#ifdef STRALLOC_H
int hmap_set_stralloc(HMAP_DB **hmap_db, const stralloc *key, const stralloc *data);
#endif

#ifdef BUFFER_H
void hmap_dump(HMAP_DB *my_hmap_db, buffer*);
#endif

#endif /* defined HMAP_H */
