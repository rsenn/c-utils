#include "unit_test.h"
#include "../lib/bmap.h"
#include "../lib/fmt.h"
#include "../lib/str.h"
#include "../lib/byte.h"

/*
 * bmap_init, bmap_set, bmap_get, bmap_remove, bmap_iter, bmap_next, bmap_deinit:
 * the macro generated map of rxi's "map" (bmap_t(T))
 */
TEST(test_bmap_set_get) {
  bmap_int_t m;
  int* p;

  bmap_init(&m);
  bmap_set(&m, "a", 1);
  bmap_set(&m, "b", 2);
  p = bmap_get(&m, "a");
  ASSERT_EQ(1, p != NULL);
  ASSERT_EQ(1, *p);
  ASSERT_EQ(1, bmap_get(&m, "missing") == NULL);

  bmap_set(&m, "a", 3); /* replaces */
  p = bmap_get(&m, "a");
  ASSERT_EQ(3, *p);
  ASSERT_EQ(2, m.base.nnodes);
  bmap_deinit(&m);
}

TEST(test_bmap_remove) {
  bmap_int_t m;

  bmap_init(&m);
  bmap_set(&m, "a", 1);
  bmap_set(&m, "b", 2);
  bmap_remove(&m, "a");
  ASSERT_EQ(1, bmap_get(&m, "a") == NULL);
  ASSERT_EQ(1, m.base.nnodes);
  bmap_remove(&m, "a"); /* twice is fine */
  ASSERT_EQ(1, m.base.nnodes);
  bmap_deinit(&m);
}

TEST(test_bmap_iterate_and_grow) {
  bmap_int_t m;
  bmap_iter_t it;
  const char* k;
  char key[16];
  int i, n = 0, sum = 0;

  bmap_init(&m);
  for(i = 0; i < 300; ++i) { /* more than the initial buckets: it resizes */
    size_t l = str_copy(key, "k");
    key[l + fmt_ulong(key + l, (unsigned long)i)] = 0;
    bmap_set(&m, key, i);
  }
  ASSERT_EQ(300, m.base.nnodes);
  ASSERT_EQ(150, *bmap_get(&m, "k150"));

  it = bmap_iter(&m);
  while((k = bmap_next(&m, &it))) {
    ++n;
    sum += *bmap_get(&m, k);
  }
  ASSERT_EQ(300, n);
  ASSERT_EQ(299 * 300 / 2, sum);
  bmap_deinit(&m);
  ASSERT_EQ(1, m.base.buckets == NULL);
}

TEST(test_bmap_string_values) {
  bmap_str_t m;
  char** p;

  bmap_init(&m);
  bmap_set(&m, "k", "value"); /* the pointer is stored, not the text */
  p = bmap_get(&m, "k");
  ASSERT_STR_EQUAL("value", *p);
  bmap_deinit(&m);
}

/*
 * the functions behind the macros: bmap_set_, bmap_get_, bmap_remove_, bmap_iter_, bmap_next_,
 * bmap_deinit_, and bmap_getref
 */
TEST(test_bmap_underscore_functions) {
  bmap_base_t base;
  bmap_iter_t it;
  int v = 7, *p;
  const char* k;
  bmap_node_t** ref;

  byte_zero(&base, sizeof base);
  ASSERT_EQ(NULL, bmap_getref(&base, "x")); /* nothing in an empty map */
  ASSERT_EQ(0, bmap_set_(&base, "x", &v, sizeof v));
  p = bmap_get_(&base, "x");
  ASSERT_EQ(1, p != NULL);
  ASSERT_EQ(7, *p);
  ref = bmap_getref(&base, "x");
  ASSERT_EQ(1, ref != NULL && *ref != NULL);
  ASSERT_EQ(NULL, bmap_getref(&base, "y"));

  it = bmap_iter_();
  k = bmap_next_(&base, &it);
  ASSERT_STR_EQUAL("x", k);
  ASSERT_EQ(NULL, bmap_next_(&base, &it));

  bmap_remove_(&base, "x");
  ASSERT_EQ(NULL, bmap_get_(&base, "x"));
  bmap_deinit_(&base);
}

#define RUN_BMAP_TESTS() \
  RUN(test_bmap_underscore_functions); \
  RUN(test_bmap_set_get); \
  RUN(test_bmap_remove); \
  RUN(test_bmap_iterate_and_grow); \
  RUN(test_bmap_string_values)

TESTS(bmap) { RUN_BMAP_TESTS(); }
