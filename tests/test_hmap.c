#include "unit_test.h"
#include "../lib/hmap.h"
#include "../lib/str.h"
#include "../lib/byte.h"
#include "../lib/stralloc.h"
#include <stdio.h>
#include <stdlib.h>

/* the keys in the order of insertion, separated by commas */
static void
keys(HMAP_DB* db, char* out) {
  TUPLE* t;
  size_t i = 0;

  out[0] = '\0';
  hmap_foreach(db, t) {
    if(i++)
      str_copy(out + str_len(out), ",");
    str_copyn(out + str_len(out), t->key, t->key_len);
  }
}

/*
 * int hmap_init(int, HMAP_DB**);
 * size_t hmap_size(HMAP_DB*);
 * int hmap_destroy(HMAP_DB**);
 */
TEST(test_hmap_init_destroy) {
  HMAP_DB* db = NULL;

  ASSERT_EQ(HMAP_SUCCESS, hmap_init(8, &db));
  ASSERT_NE(NULL, db);
  ASSERT_EQ(0, hmap_size(db));
  ASSERT_EQ(0, hmap_count(db));
  ASSERT_EQ(NULL, hmap_begin(db));

  /* an empty map has no table yet */
  ASSERT_EQ(NULL, db->table);
  ASSERT_EQ(HMAP_TUPLE_NOT_FOUND, hmap_search(db, "x", 1, NULL));
  ASSERT_EQ(NULL, hmap_get(db, "x", 1));
  ASSERT_EQ(HMAP_TUPLE_NOT_FOUND, hmap_delete(&db, "x", 1));

  ASSERT_EQ(0, hmap_destroy(&db));
  ASSERT_EQ(NULL, db);
  ASSERT_EQ(0, hmap_destroy(&db));

  /* a size of 0 or less is taken as the default */
  ASSERT_EQ(HMAP_SUCCESS, hmap_init(0, &db));
  ASSERT_GT(db->bucket_size, 0);
  hmap_destroy(&db);
}

/*
 * int hmap_add(HMAP_DB**, const void*, size_t, int, int, ...);
 * int hmap_search(HMAP_DB*, const void*, size_t, TUPLE**);
 */
TEST(test_hmap_add_search) {
  HMAP_DB* db = NULL;
  TUPLE* t = NULL;

  hmap_init(4, &db);

  ASSERT_GE(hmap_add(&db, "one", 3, 0, HMAP_DATA_TYPE_INT, 1), 0);
  ASSERT_GE(hmap_add(&db, "two", 3, 0, HMAP_DATA_TYPE_UINT, 2u), 0);
  ASSERT_GE(hmap_add(&db, "three", 5, 0, HMAP_DATA_TYPE_INT64, 3L), 0);
  ASSERT_GE(hmap_add(&db, "four", 4, 0, HMAP_DATA_TYPE_UINT64, 4UL), 0);
  ASSERT_GE(hmap_add(&db, "five", 4, 0, HMAP_DATA_TYPE_DOUBLE, 5.5), 0);
  ASSERT_GE(hmap_add(&db, "six", 3, 0, HMAP_DATA_TYPE_CHARS, "sechs", (size_t)5), 0);
  ASSERT_GE(hmap_add(&db, "seven", 5, 0, HMAP_DATA_TYPE_CUSTOM, (void*)db), 0);
  ASSERT_EQ(7, hmap_size(db));

  ASSERT_EQ(HMAP_SUCCESS, hmap_search(db, "one", 3, &t));
  ASSERT_EQ(1, t->vals.val_int);
  ASSERT_EQ(3, t->key_len);
  ASSERT_STR_EQUAL("one", t->key);
  ASSERT_EQ(HMAP_SUCCESS, hmap_search(db, "two", 3, &t));
  ASSERT_EQ(2, t->vals.val_uint);
  ASSERT_EQ(HMAP_SUCCESS, hmap_search(db, "three", 5, &t));
  ASSERT_EQ(3, t->vals.val_longlong);
  ASSERT_EQ(HMAP_SUCCESS, hmap_search(db, "four", 4, &t));
  ASSERT_EQ(4, t->vals.val_ulonglong);
  ASSERT_EQ(HMAP_SUCCESS, hmap_search(db, "five", 4, &t));
  ASSERT_EQ(1, t->vals.val_double == 5.5);
  ASSERT_EQ(HMAP_SUCCESS, hmap_search(db, "six", 3, &t));
  ASSERT_STR_EQUAL("sechs", t->vals.val_chars);
  ASSERT_EQ(5, t->data_len);
  ASSERT_EQ((void*)db, hmap_get(db, "seven", 5));
  ASSERT_STR_EQUAL("sechs", hmap_get(db, "six", 3));

  /* a key that is there says so and keeps its value */
  ASSERT_EQ(HMAP_TUPLE_ALREADY_EXIST, hmap_add(&db, "one", 3, 0, HMAP_DATA_TYPE_INT, 99));
  hmap_search(db, "one", 3, &t);
  ASSERT_EQ(1, t->vals.val_int);
  ASSERT_EQ(7, hmap_size(db));

  /* the key has to match in full */
  ASSERT_EQ(HMAP_TUPLE_NOT_FOUND, hmap_search(db, "on", 2, NULL));
  ASSERT_EQ(HMAP_TUPLE_NOT_FOUND, hmap_search(db, "onee", 4, NULL));
  ASSERT_EQ(HMAP_SUCCESS, hmap_search(db, "one", 3, NULL));

  hmap_destroy(&db);
}

TEST(test_hmap_bad_arguments) {
  HMAP_DB* db = NULL;

  hmap_init(4, &db);

  ASSERT_EQ(HMAP_KEY_EMPTY, hmap_add(&db, NULL, 3, 0, HMAP_DATA_TYPE_INT, 1));
  ASSERT_EQ(HMAP_KEY_EMPTY, hmap_add(&db, "x", 0, 0, HMAP_DATA_TYPE_INT, 1));
  ASSERT_EQ(HMAP_DATA_TYPE_INVALID, hmap_add(&db, "x", 1, 0, 77, 1));
  ASSERT_EQ(0, hmap_size(db));
  ASSERT_EQ(HMAP_KEY_EMPTY, hmap_search(db, NULL, 1, NULL));
  ASSERT_EQ(HMAP_DB_EMPTY, hmap_search(NULL, "x", 1, NULL));

  hmap_destroy(&db);
}

/* the entries come in the order they were added, also when the table grows and entries go */
TEST(test_hmap_order_and_growth) {
  HMAP_DB* db = NULL;
  char key[16], out[8192], expect[8192];
  int i, n = 3000;

  hmap_init(4, &db);
  expect[0] = '\0';

  for(i = 0; i < n; i++) {
    snprintf(key, sizeof(key), "k%d", i);
    ASSERT_GE(hmap_add(&db, key, str_len(key), 0, HMAP_DATA_TYPE_INT, i), 0);
    if(i < 8) {
      if(i)
        str_copy(expect + str_len(expect), ",");
      str_copy(expect + str_len(expect), key);
    }
  }
  ASSERT_EQ(n, hmap_size(db));
  ASSERT_GT(db->bucket_size, 4);
  ASSERT_GE(db->bucket_size, n / 2);

  for(i = 0; i < n; i++) {
    TUPLE* t;

    snprintf(key, sizeof(key), "k%d", i);
    ASSERT_EQ(HMAP_SUCCESS, hmap_search(db, key, str_len(key), &t));
    ASSERT_EQ(i, t->vals.val_int);
  }

  /* the first ones, in order */
  {
    TUPLE* t = hmap_begin(db);

    for(i = 0; i < 8; i++, t = t->next) {
      snprintf(key, sizeof(key), "k%d", i);
      ASSERT_STR_EQUAL(key, t->key);
    }
  }
  (void)out;
  hmap_destroy(&db);
}

/*
 * int hmap_delete(HMAP_DB**, const void*, size_t);
 */
TEST(test_hmap_delete) {
  HMAP_DB* db = NULL;
  char out[64], key[16];
  int i;

  hmap_init(2, &db);
  hmap_add(&db, "a", 1, 0, HMAP_DATA_TYPE_INT, 1);
  hmap_add(&db, "b", 1, 0, HMAP_DATA_TYPE_INT, 2);
  hmap_add(&db, "c", 1, 0, HMAP_DATA_TYPE_CHARS, "cc", (size_t)2);
  hmap_add(&db, "d", 1, 0, HMAP_DATA_TYPE_INT, 4);

  ASSERT_EQ(HMAP_TUPLE_NOT_FOUND, hmap_delete(&db, "x", 1));
  ASSERT_EQ(HMAP_SUCCESS, hmap_delete(&db, "b", 1));
  ASSERT_EQ(3, hmap_size(db));
  keys(db, out);
  ASSERT_STR_EQUAL("a,c,d", out);
  ASSERT_EQ(HMAP_TUPLE_NOT_FOUND, hmap_search(db, "b", 1, NULL));
  ASSERT_EQ(HMAP_TUPLE_NOT_FOUND, hmap_delete(&db, "b", 1));

  ASSERT_EQ(HMAP_SUCCESS, hmap_delete(&db, "a", 1)); /* the first */
  keys(db, out);
  ASSERT_STR_EQUAL("c,d", out);
  ASSERT_EQ(HMAP_SUCCESS, hmap_delete(&db, "c", 1)); /* one that owns memory */
  ASSERT_EQ(HMAP_SUCCESS, hmap_delete(&db, "d", 1)); /* the last */
  ASSERT_EQ(0, hmap_size(db));
  ASSERT_EQ(NULL, hmap_begin(db));

  /* again after it was empty */
  ASSERT_GE(hmap_add(&db, "e", 1, 0, HMAP_DATA_TYPE_INT, 5), 0);
  ASSERT_EQ(1, hmap_size(db));

  /* many: take out half, the rest must be found */
  for(i = 0; i < 500; i++) {
    snprintf(key, sizeof(key), "n%d", i);
    hmap_add(&db, key, str_len(key), 0, HMAP_DATA_TYPE_INT, i);
  }
  for(i = 0; i < 500; i += 2) {
    snprintf(key, sizeof(key), "n%d", i);
    ASSERT_EQ(HMAP_SUCCESS, hmap_delete(&db, key, str_len(key)));
  }
  ASSERT_EQ(251, hmap_size(db));
  for(i = 0; i < 500; i++) {
    snprintf(key, sizeof(key), "n%d", i);
    ASSERT_EQ(i % 2 ? HMAP_SUCCESS : HMAP_TUPLE_NOT_FOUND, hmap_search(db, key, str_len(key), NULL));
  }
  hmap_destroy(&db);
}

/*
 * TUPLE* hmap_set(HMAP_DB**, const void*, size_t, void*, size_t);
 * void* hmap_get(HMAP_DB*, const char*, size_t);
 */
TEST(test_hmap_set_get) {
  HMAP_DB* db = NULL;
  TUPLE* t;

  hmap_init(4, &db);

  t = hmap_set(&db, "key", 3, "value", 5);
  ASSERT_NE(NULL, t);
  ASSERT_STR_EQUAL("value", hmap_get(db, "key", 3));
  ASSERT_EQ(5, t->data_len);
  ASSERT_EQ(1, hmap_size(db));

  /* set again: the value is replaced, the entry stays */
  ASSERT_EQ(t, hmap_set(&db, "key", 3, "other value", 11));
  ASSERT_STR_EQUAL("other value", hmap_get(db, "key", 3));
  ASSERT_EQ(11, t->data_len);
  ASSERT_EQ(1, hmap_size(db));

  /* the value that is set for an entry of another type */
  hmap_add(&db, "num", 3, 0, HMAP_DATA_TYPE_INT, 5);
  t = hmap_set(&db, "num", 3, "text", 4);
  ASSERT_NE(NULL, t);
  ASSERT_EQ(HMAP_DATA_TYPE_CHARS, t->data_type);
  ASSERT_STR_EQUAL("text", hmap_get(db, "num", 3));

  /* a value with bytes and a NUL in it */
  hmap_set(&db, "bin", 3, "a\0b", 3);
  t = NULL;
  hmap_search(db, "bin", 3, &t);
  ASSERT_EQ(3, t->data_len);
  ASSERT_EQ(0, byte_diff(t->vals.val_chars, 3, "a\0b"));

  /* nothing for no key */
  ASSERT_EQ(NULL, hmap_set(&db, "", 0, "x", 1));
  ASSERT_EQ(NULL, hmap_get(db, "nope", 4));

  hmap_destroy(&db);
}

/* keys of any length, they were limited to 1023 bytes and that was not checked */
TEST(test_hmap_long_keys) {
  HMAP_DB* db = NULL;
  char* big = malloc(100001);
  TUPLE* t;

  byte_fill(big, 100000, 'K');
  big[100000] = '\0';
  hmap_init(4, &db);

  ASSERT_GE(hmap_add(&db, big, 100000, 0, HMAP_DATA_TYPE_INT, 1), 0);
  big[99999] = 'L';
  ASSERT_GE(hmap_add(&db, big, 100000, 0, HMAP_DATA_TYPE_INT, 2), 0);
  ASSERT_GE(hmap_add(&db, "short", 5, 0, HMAP_DATA_TYPE_INT, 3), 0);

  ASSERT_EQ(HMAP_SUCCESS, hmap_search(db, big, 100000, &t));
  ASSERT_EQ(2, t->vals.val_int);
  ASSERT_EQ(100000, t->key_len);
  big[99999] = 'K';
  ASSERT_EQ(HMAP_SUCCESS, hmap_search(db, big, 100000, &t));
  ASSERT_EQ(1, t->vals.val_int);
  ASSERT_EQ(HMAP_SUCCESS, hmap_search(db, "short", 5, &t));
  ASSERT_EQ(3, t->vals.val_int);

  free(big);
  hmap_destroy(&db);
}

/* keys are bytes: a NUL is a part of the key */
TEST(test_hmap_binary_keys) {
  HMAP_DB* db = NULL;

  hmap_init(4, &db);
  ASSERT_GE(hmap_add(&db, "a\0b", 3, 0, HMAP_DATA_TYPE_INT, 1), 0);
  ASSERT_GE(hmap_add(&db, "a\0c", 3, 0, HMAP_DATA_TYPE_INT, 2), 0);
  ASSERT_GE(hmap_add(&db, "a", 1, 0, HMAP_DATA_TYPE_INT, 3), 0);
  ASSERT_EQ(3, hmap_size(db));
  ASSERT_EQ(HMAP_SUCCESS, hmap_search(db, "a\0c", 3, NULL));
  ASSERT_EQ(HMAP_TUPLE_NOT_FOUND, hmap_search(db, "a\0d", 3, NULL));
  hmap_destroy(&db);
}

/*
 * int hmap_truncate(HMAP_DB**);
 */
TEST(test_hmap_truncate) {
  HMAP_DB* db = NULL;
  int i;
  char key[16];

  hmap_init(4, &db);
  for(i = 0; i < 100; i++) {
    snprintf(key, sizeof(key), "k%d", i);
    hmap_add(&db, key, str_len(key), 0, HMAP_DATA_TYPE_CHARS, key, (size_t)str_len(key));
  }
  ASSERT_EQ(100, hmap_size(db));
  hmap_truncate(&db);
  ASSERT_EQ(0, hmap_size(db));
  ASSERT_EQ(NULL, hmap_begin(db));
  ASSERT_EQ(HMAP_TUPLE_NOT_FOUND, hmap_search(db, "k5", 2, NULL));

  /* usable after that */
  ASSERT_GE(hmap_add(&db, "k5", 2, 0, HMAP_DATA_TYPE_INT, 5), 0);
  ASSERT_EQ(1, hmap_size(db));
  hmap_destroy(&db);
}

/*
 * int hmap_set_stralloc(HMAP_DB**, const stralloc*, const stralloc*);
 */
TEST(test_hmap_set_stralloc) {
  HMAP_DB* db = NULL;
  stralloc k, v, empty;

  stralloc_init(&k);
  stralloc_init(&v);
  stralloc_init(&empty);
  stralloc_copys(&k, "name");
  stralloc_copys(&v, "value");
  hmap_init(4, &db);

  ASSERT_GE(hmap_set_stralloc(&db, &k, &v), 0);
  ASSERT_STR_EQUAL("value", hmap_get(db, "name", 4));
  ASSERT_EQ(HMAP_TUPLE_ALREADY_EXIST, hmap_set_stralloc(&db, &k, &v));

  /* no value, no key: an error code and nothing added */
  ASSERT_LT(hmap_set_stralloc(&db, &k, &empty), 0);
  ASSERT_LT(hmap_set_stralloc(&db, &empty, &v), 0);
  ASSERT_EQ(1, hmap_size(db));

  stralloc_free(&k);
  stralloc_free(&v);
  hmap_destroy(&db);
}

/*
 * int hmap_free_data(TUPLE*);
 */
TEST(test_hmap_free_data) {
  HMAP_DB* db = NULL;
  TUPLE* t;

  hmap_init(4, &db);
  hmap_add(&db, "c", 1, 0, HMAP_DATA_TYPE_CHARS, "text", (size_t)4);
  hmap_add(&db, "i", 1, 0, HMAP_DATA_TYPE_INT, 5);
  hmap_search(db, "c", 1, &t);
  ASSERT_EQ(HMAP_SUCCESS, hmap_free_data(t));
  ASSERT_EQ(NULL, t->vals.val_chars);
  ASSERT_EQ(0, t->data_len);
  hmap_search(db, "i", 1, &t);
  ASSERT_EQ(HMAP_SUCCESS, hmap_free_data(t));
  ASSERT_EQ(0, t->vals.val_int);
  /* the entry can still be deleted: nothing is freed twice */
  ASSERT_EQ(HMAP_SUCCESS, hmap_delete(&db, "c", 1));
  hmap_destroy(&db);
}

#define RUN_HMAP_TESTS() \
  RUN(test_hmap_init_destroy); \
  RUN(test_hmap_add_search); \
  RUN(test_hmap_bad_arguments); \
  RUN(test_hmap_order_and_growth); \
  RUN(test_hmap_delete); \
  RUN(test_hmap_set_get); \
  RUN(test_hmap_long_keys); \
  RUN(test_hmap_binary_keys); \
  RUN(test_hmap_truncate); \
  RUN(test_hmap_set_stralloc); \
  RUN(test_hmap_free_data)

TESTS(hmap) { RUN_HMAP_TESTS(); }
