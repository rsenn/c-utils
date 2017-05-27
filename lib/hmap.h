#ifndef _HMAP_H
# define _HMAP_H
 
# define MAP_BUCKET   1024
# define MAX_SIZE_KEY 1024

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

enum hmap_tuple_type {
  HMAP_TUPLE_PRIMARY = 0,
  HMAP_TUPLE_SECONDARY
};

enum hmap_data_type {
  HMAP_DATA_TYPE_INT = 0,
  HMAP_DATA_TYPE_UINT,
  HMAP_DATA_TYPE_INT64,
  HMAP_DATA_TYPE_UNIT64,
  HMAP_DATA_TYPE_DOUBLE,
  HMAP_DATA_TYPE_CHARS,
  HMAP_DATA_TYPE_CUSTOM,
  HMAP_DATA_TYPE_TUPLE,
} HMAP_DATA_TYPE;

typedef struct tuple {
  struct tuple *next;
  struct tuple *prev;
  struct tuple *hash_next;
  struct tuple *hash_prev;

  char key[MAX_SIZE_KEY];
  int key_len;
  int type;
  int index;
  int data_len;
  void *data;

  int data_type;
  union {
	int val_int;
	unsigned int val_uint;
	long int val_int64;
	unsigned long int val_uint64;
	double val_double;
	char *val_chars;
	struct tuple *val_tuple;
	void *val_custom;
  } vals;

} TUPLE;

typedef struct hmap_db {
  int tuple_count;
  int secondary_tuple_count;
  int bucket_size;
  TUPLE *tuple;
  TUPLE *list_tuple;
} HMAP_DB;

int hmap_init(int bucket_size, HMAP_DB **hmap_db);
int hmap_is_locate(HMAP_DB *hmap_db, void *key, int k_len);
int hmap_add(HMAP_DB **hmap_db, void *key, int k_len, int dup_flag, int data_type, ...);
int hmap_add_tuple_with_data(HMAP_DB **hmap_db, void *key, int k_len, int data_type, void *data, int dup_flag);
int hmap_delete(HMAP_DB **hmap_db, void *key, int k_len);
int hmap_set(HMAP_DB *hmap_db, void *key, int k_len, void *data, int d_len, int alway_set);
int hmap_print_table(HMAP_DB *hmap_db);
int hmap_print_list(HMAP_DB *hmap_db);
int hmap_print_tree(HMAP_DB *my_hmap_db);
int hmap_search(HMAP_DB *hmap_db, void *key, int k_len, TUPLE **data);
int hmap_destroy(HMAP_DB **hmap_db);
int hmap_truncate(HMAP_DB **HMAP_db);

#endif