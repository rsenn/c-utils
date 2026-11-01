#include "unit_test.h"
#include "../lib/cbmap.h"
#include "../lib/str.h"
#include "../lib/byte.h"
#include <stdio.h>
#include <stdlib.h>

static int calls;
static char order[64];

static int
collect(const void* key, size_t key_len, const void* value, size_t value_len, void* user) {
  (void)value_len;
  (void)user;
  calls++;
  str_copyn(order + str_len(order), (const char*)key, key_len);
  str_copy(order + str_len(order), "=");
  str_copy(order + str_len(order), (const char*)value);
  str_copy(order + str_len(order), ";");
  return 1;
}

static int
stop_first(const void* key, size_t key_len, const void* value, size_t value_len, void* user) {
  (void)key;
  (void)key_len;
  (void)value;
  (void)value_len;
  (void)user;
  return ++calls < 2;
}

/*
 * cbmap_t cbmap_new(void);
 * size_t cbmap_count(cbmap_t);
 * void cbmap_destroy(cbmap_t*);
 */
TEST(test_cbmap_new_destroy) {
  cbmap_t m = cbmap_new();

  ASSERT_NE(NULL, m);
  ASSERT_EQ(0, cbmap_count(m));
  cbmap_destroy(&m);
  ASSERT_EQ(NULL, m);

  /* destroying nothing is allowed */
  cbmap_destroy(&m);
  cbmap_destroy(NULL);
}

/*
 * int cbmap_insert(cbmap_t, void*, size_t, void*, size_t);
 * int cbmap_get(cbmap_t, void*, size_t, void**, size_t*);
 */
TEST(test_cbmap_insert_get) {
  cbmap_t m = cbmap_new();
  void* v = NULL;
  size_t vl = 0;
  char key[] = "alpha", val[] = "one";

  ASSERT_EQ(1, cbmap_insert(m, key, 5, val, 4));
  ASSERT_EQ(1, cbmap_count(m));

  ASSERT_EQ(1, cbmap_get(m, "alpha", 5, &v, &vl));
  ASSERT_STR_EQUAL("one", v);
  ASSERT_EQ(4, vl);

  /* the map keeps copies */
  key[0] = 'X';
  val[0] = 'X';
  ASSERT_EQ(1, cbmap_get(m, "alpha", 5, &v, &vl));
  ASSERT_STR_EQUAL("one", v);
  ASSERT_EQ(0, cbmap_get(m, key, 5, &v, &vl));

  /* an update returns 2 and does not add */
  ASSERT_EQ(2, cbmap_insert(m, "alpha", 5, "uno!", 5));
  ASSERT_EQ(1, cbmap_count(m));
  ASSERT_EQ(1, cbmap_get(m, "alpha", 5, &v, &vl));
  ASSERT_STR_EQUAL("uno!", v);
  ASSERT_EQ(5, vl);

  ASSERT_EQ(0, cbmap_get(m, "beta", 4, &v, &vl));
  /* the value pointers are optional */
  ASSERT_EQ(1, cbmap_get(m, "alpha", 5, NULL, NULL));

  cbmap_destroy(&m);
}

/*
 * int cbmap_delete(cbmap_t, void*, size_t);
 */
TEST(test_cbmap_delete) {
  cbmap_t m = cbmap_new();
  char k[16];
  int i;

  ASSERT_EQ(0, cbmap_delete(m, "x", 1));

  for(i = 0; i < 100; i++) {
    snprintf(k, sizeof(k), "key%02d", i);
    ASSERT_EQ(1, cbmap_insert(m, k, 5, "v", 2));
  }
  ASSERT_EQ(100, cbmap_count(m));

  for(i = 0; i < 100; i += 2) {
    snprintf(k, sizeof(k), "key%02d", i);
    ASSERT_EQ(1, cbmap_delete(m, k, 5));
  }
  ASSERT_EQ(50, cbmap_count(m));
  ASSERT_EQ(0, cbmap_delete(m, "key00", 5));
  ASSERT_EQ(0, cbmap_get(m, "key00", 5, NULL, NULL));
  ASSERT_EQ(1, cbmap_get(m, "key01", 5, NULL, NULL));

  for(i = 1; i < 100; i += 2) {
    snprintf(k, sizeof(k), "key%02d", i);
    ASSERT_EQ(1, cbmap_delete(m, k, 5));
  }
  ASSERT_EQ(0, cbmap_count(m));

  /* the map is usable after it was emptied */
  ASSERT_EQ(1, cbmap_insert(m, "again", 5, "v", 2));
  ASSERT_EQ(1, cbmap_count(m));
  cbmap_destroy(&m);
}

/*
 * int cbmap_visit_all(cbmap_t, cbmap_visitor, void*);
 * int cbmap_visit_prefix(cbmap_t, uint8*, size_t, cbmap_visitor, void*);
 */
TEST(test_cbmap_visit) {
  cbmap_t m = cbmap_new();

  /* nothing to visit is a complete visit */
  calls = 0;
  ASSERT_EQ(1, cbmap_visit_all(m, collect, NULL));
  ASSERT_EQ(0, calls);

  cbmap_insert(m, "b", 1, "2", 2);
  cbmap_insert(m, "a", 1, "1", 2);
  cbmap_insert(m, "ab", 2, "12", 3);
  cbmap_insert(m, "c", 1, "3", 2);

  /* in the order of the keys */
  calls = 0;
  order[0] = '\0';
  ASSERT_EQ(1, cbmap_visit_all(m, collect, NULL));
  ASSERT_EQ(4, calls);
  ASSERT_STR_EQUAL("a=1;ab=12;b=2;c=3;", order);

  calls = 0;
  order[0] = '\0';
  ASSERT_EQ(1, cbmap_visit_prefix(m, (uint8*)"a", 1, collect, NULL));
  ASSERT_EQ(2, calls);
  ASSERT_STR_EQUAL("a=1;ab=12;", order);

  calls = 0;
  ASSERT_EQ(1, cbmap_visit_prefix(m, (uint8*)"zz", 2, collect, NULL));
  ASSERT_EQ(0, calls);

  /* the visitor stops the walk by not returning 1, and the call says so */
  calls = 0;
  ASSERT_EQ(0, cbmap_visit_all(m, stop_first, NULL));
  ASSERT_EQ(2, calls);

  cbmap_destroy(&m);
}

/* a key that is the start of another key, in either order */
TEST(test_cbmap_prefix_keys) {
  cbmap_t m = cbmap_new();
  void* v;
  size_t vl;

  ASSERT_EQ(1, cbmap_insert(m, "ab", 2, "long", 5));
  ASSERT_EQ(1, cbmap_insert(m, "a", 1, "short", 6));
  ASSERT_EQ(1, cbmap_insert(m, "abc", 3, "longer", 7));
  ASSERT_EQ(1, cbmap_insert(m, "b", 1, "other", 6));
  ASSERT_EQ(4, cbmap_count(m));

  ASSERT_EQ(1, cbmap_get(m, "a", 1, &v, &vl));
  ASSERT_STR_EQUAL("short", v);
  ASSERT_EQ(1, cbmap_get(m, "ab", 2, &v, &vl));
  ASSERT_STR_EQUAL("long", v);
  ASSERT_EQ(1, cbmap_get(m, "abc", 3, &v, &vl));
  ASSERT_STR_EQUAL("longer", v);
  ASSERT_EQ(0, cbmap_get(m, "abcd", 4, &v, &vl));

  /* updating one does not touch the others */
  ASSERT_EQ(2, cbmap_insert(m, "a", 1, "new", 4));
  ASSERT_EQ(4, cbmap_count(m));
  ASSERT_EQ(1, cbmap_get(m, "ab", 2, &v, &vl));
  ASSERT_STR_EQUAL("long", v);

  ASSERT_EQ(1, cbmap_delete(m, "ab", 2));
  ASSERT_EQ(1, cbmap_get(m, "a", 1, &v, &vl));
  ASSERT_EQ(1, cbmap_get(m, "abc", 3, &v, &vl));
  ASSERT_EQ(0, cbmap_get(m, "ab", 2, &v, &vl));

  /* keys that differ only by zero bytes at the end cannot both be kept */
  ASSERT_EQ(0, cbmap_insert(m, "a\0", 2, "x", 2));
  ASSERT_EQ(3, cbmap_count(m));
  cbmap_destroy(&m);
}

/*
 * cbmap_t cbmap_new_with_static_keys_and_values(void);
 */
TEST(test_cbmap_static) {
  cbmap_t m = cbmap_new_with_static_keys_and_values();
  static char key[] = "key", val[] = "value";
  void* v = NULL;

  ASSERT_EQ(1, cbmap_insert(m, key, 3, val, 6));
  ASSERT_EQ(1, cbmap_get(m, "key", 3, &v, NULL));
  /* the very same memory, no copy */
  ASSERT_EQ((void*)val, v);
  cbmap_destroy(&m);
}

static int allocs, frees;

static void*
my_alloc(void* data, size_t len) {
  void* p = malloc(len);

  allocs++;
  byte_copy(p, len, data);
  return p;
}

static void
my_free(void* data) {
  frees++;
  free(data);
}

/*
 * cbmap_t cbmap_new_with_custom_allocation(...);
 */
TEST(test_cbmap_custom_allocation) {
  cbmap_t m;

  allocs = frees = 0;
  m = cbmap_new_with_custom_allocation(my_alloc, my_free, my_alloc, my_free);
  ASSERT_NE(NULL, m);

  cbmap_insert(m, "k1", 2, "v1", 3);
  cbmap_insert(m, "k2", 2, "v2", 3);
  ASSERT_EQ(4, allocs);

  cbmap_delete(m, "k1", 2);
  ASSERT_EQ(2, frees);

  cbmap_destroy(&m);
  ASSERT_EQ(4, frees);
  ASSERT_EQ(allocs, frees);
}

/* a lot of keys of different lengths, with binary content */
TEST(test_cbmap_many) {
  cbmap_t m = cbmap_new();
  int i;
  unsigned char key[4];
  void* v;
  size_t vl;

  for(i = 0; i < 1000; i++) {
    key[0] = (unsigned char)(i & 0xff);
    key[1] = (unsigned char)(i >> 8);
    key[2] = 0;
    key[3] = (unsigned char)(i % 7);
    ASSERT_EQ(1, cbmap_insert(m, key, 4, &i, sizeof(i)));
  }
  ASSERT_EQ(1000, cbmap_count(m));

  for(i = 0; i < 1000; i++) {
    key[0] = (unsigned char)(i & 0xff);
    key[1] = (unsigned char)(i >> 8);
    key[2] = 0;
    key[3] = (unsigned char)(i % 7);
    ASSERT_EQ(1, cbmap_get(m, key, 4, &v, &vl));
    ASSERT_EQ(sizeof(int), vl);
    ASSERT_EQ(i, *(int*)v);
  }

  /* a shorter key is another key */
  ASSERT_EQ(0, cbmap_get(m, "\x01\x00\x00", 3, &v, &vl));
  cbmap_destroy(&m);
}

#define RUN_CBMAP_TESTS() \
  RUN(test_cbmap_new_destroy); \
  RUN(test_cbmap_insert_get); \
  RUN(test_cbmap_delete); \
  RUN(test_cbmap_visit); \
  RUN(test_cbmap_prefix_keys); \
  RUN(test_cbmap_static); \
  RUN(test_cbmap_custom_allocation); \
  RUN(test_cbmap_many)

TESTS(cbmap) { RUN_CBMAP_TESTS(); }
