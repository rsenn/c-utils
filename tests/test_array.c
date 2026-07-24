#include "unit_test.h"
#include "../lib/array.h"
#include "../lib/byte.h"

/*
 * void* array_allocate(array* x, uint64 membersize, int64 pos);
 */
TEST(test_array_allocate) {
  array a = ARRAY_INIT();
  int* p = array_allocate(&a, sizeof(int), 3);

  ASSERT_NE(0, (size_t)p);
  ASSERT_GE(a.initialized, 4 * sizeof(int));

  array_reset(&a);
}

/*
 * void* array_get(const array* const x, uint64 membersize, int64 pos);
 */
TEST(test_array_get) {
  array a = ARRAY_INIT();
  /* array_allocate(x, membersize, pos) returns a pointer to element
     `pos` itself, not the array base. */
  int* p3 = array_allocate(&a, sizeof(int), 3);

  *p3 = 42;
  ASSERT_EQ((size_t)p3, (size_t)array_get(&a, sizeof(int), 3));
  ASSERT_EQ(42, *(int*)array_get(&a, sizeof(int), 3));
  ASSERT_EQ(0, (size_t)array_get(&a, sizeof(int), 10));

  array_reset(&a);
}

/*
 * void* array_start(const array* const);
 */
TEST(test_array_start) {
  array a = ARRAY_INIT();

  array_allocate(&a, sizeof(char), 3);
  ASSERT_EQ((size_t)a.p, (size_t)array_start(&a));

  array_reset(&a);
}

/*
 * void* array_end(const array* const);
 */
TEST(test_array_end) {
  array a = ARRAY_INIT();

  array_catb(&a, "abcd", 4);
  ASSERT_EQ((size_t)(a.p + a.initialized), (size_t)array_end(&a));

  array_reset(&a);
}

/*
 * int64 array_length(const array* const x, uint64 membersize);
 */
TEST(test_array_length) {
  array a = ARRAY_INIT();

  array_allocate(&a, sizeof(int), 4);
  ASSERT_EQ(5, array_length(&a, sizeof(int)));

  array_reset(&a);
}

/*
 * int64 array_bytes(const array* const x);
 */
TEST(test_array_bytes) {
  array a = ARRAY_INIT();

  array_catb(&a, "abcde", 5);
  ASSERT_EQ(5, array_bytes(&a));

  array_reset(&a);
}

/*
 * void array_truncate(array* x, uint64 membersize, int64 len);
 */
TEST(test_array_truncate) {
  array a = ARRAY_INIT();

  array_catb(&a, "abcdef", 6);
  array_truncate(&a, 1, 3);
  ASSERT_EQ(3, array_bytes(&a));

  array_reset(&a);
}

/*
 * void array_trunc(array* x);
 */
TEST(test_array_trunc) {
  array a = ARRAY_INIT();

  array_catb(&a, "abcdef", 6);
  array_trunc(&a);
  ASSERT_EQ(0, array_bytes(&a));

  array_reset(&a);
}

/*
 * void array_reset(array* x);
 */
TEST(test_array_reset) {
  array a = ARRAY_INIT();

  array_catb(&a, "abc", 3);
  array_reset(&a);

  ASSERT_EQ(0, (size_t)a.p);
  ASSERT_EQ(0, a.allocated);
  ASSERT_EQ(0, a.initialized);
}

/*
 * void array_fail(array* x);
 */
TEST(test_array_fail) {
  array a = ARRAY_INIT();

  array_fail(&a);
  ASSERT_EQ(1, array_failed(&a));
}

/*
 * int array_equal(const array* const x, const array* const y);
 */
TEST(test_array_equal) {
  array a = ARRAY_INIT(), b = ARRAY_INIT();

  array_cats(&a, "hello");
  array_cats(&b, "hello");
  ASSERT_NE(0, array_equal(&a, &b));

  array_cats(&b, "!");
  ASSERT_EQ(0, array_equal(&a, &b));

  array_reset(&a);
  array_reset(&b);
}

/*
 * void array_cat(array* to, const array* const from);
 */
TEST(test_array_cat) {
  array a = ARRAY_INIT(), b = ARRAY_INIT();

  array_cats(&a, "foo");
  array_cats(&b, "bar");
  array_cat(&a, &b);

  ASSERT_EQ(6, array_bytes(&a));
  ASSERT_EQ(0, byte_diff(array_start(&a), 6, "foobar"));

  array_reset(&a);
  array_reset(&b);
}

/*
 * void array_catb(array* to, const void* from, uint64 len);
 */
TEST(test_array_catb) {
  array a = ARRAY_INIT();

  array_catb(&a, "abc", 3);
  ASSERT_EQ(3, array_bytes(&a));
  ASSERT_EQ(0, byte_diff(array_start(&a), 3, "abc"));

  array_reset(&a);
}

/*
 * void array_cats(array* to, const char* from);
 */
TEST(test_array_cats) {
  array a = ARRAY_INIT();

  array_cats(&a, "hello");
  ASSERT_EQ(5, array_bytes(&a));
  ASSERT_EQ(0, byte_diff(array_start(&a), 5, "hello"));

  array_reset(&a);
}

/*
 * void array_cats0(array* to, const char* from);
 */
TEST(test_array_cats0) {
  array a = ARRAY_INIT();

  array_cats0(&a, "hi");
  ASSERT_EQ(3, array_bytes(&a));
  ASSERT_EQ(0, byte_diff(array_start(&a), 3, "hi\0"));

  array_reset(&a);
}

/*
 * void array_cat0(array* to);
 */
TEST(test_array_cat0) {
  array a = ARRAY_INIT();

  array_cats(&a, "hi");
  array_cat0(&a);
  ASSERT_EQ(3, array_bytes(&a));
  ASSERT_EQ(0, ((char*)array_start(&a))[2]);

  array_reset(&a);
}

/*
 * void array_cate(array* to, const array* const from, int64 pos, int64 stop);
 */
TEST(test_array_cate) {
  array a = ARRAY_INIT(), b = ARRAY_INIT();

  array_cats(&b, "hello world");
  array_cate(&a, &b, 6, 11);

  ASSERT_EQ(5, array_bytes(&a));
  ASSERT_EQ(0, byte_diff(array_start(&a), 5, "world"));

  array_reset(&a);
  array_reset(&b);
}

/*
 * void* array_find(array*, uint64 membersize, void* data);
 */
TEST(test_array_find) {
  array a = ARRAY_INIT();
  int vals[3] = {10, 20, 30};

  array_catb(&a, vals, sizeof(vals));

  int want = 20;
  void* p = array_find(&a, sizeof(int), &want);

  ASSERT_NE(0, (size_t)p);
  ASSERT_EQ(20, *(int*)p);

  int missing = 99;
  ASSERT_EQ(0, (size_t)array_find(&a, sizeof(int), &missing));

  array_reset(&a);
}

/*
 * int64 array_indexof(array*, uint64 membersize, void* data);
 */
TEST(test_array_indexof) {
  array a = ARRAY_INIT();
  int vals[3] = {10, 20, 30};

  array_catb(&a, vals, sizeof(vals));

  int want = 30;
  ASSERT_EQ(2, array_indexof(&a, sizeof(int), &want));

  int missing = 99;
  ASSERT_EQ(-1, array_indexof(&a, sizeof(int), &missing));

  array_reset(&a);
}

/*
 * void array_shift(array* x, uint64 membersize, uint64 members);
 *
 * Not tested: declared in array.h but has no implementation anywhere in
 * the tree (would fail to link). See BUGS.
 */
TEST(test_array_shift) {}

/*
 * void array_chop(array* x, uint64 membersize, uint64 members);
 *
 * Not tested: declared in array.h but has no implementation anywhere in
 * the tree (would fail to link). See BUGS.
 */
TEST(test_array_chop) {}

/*
 * int64 array_splice(array*, uint64 membersize, uint64 start, uint64 del, uint64 insert, const void* x);
 */
TEST(test_array_splice) {
  array a = ARRAY_INIT();

  array_cats(&a, "hello world");
  array_splice(&a, 1, 6, 5, 5, "there");

  ASSERT_EQ(11, array_bytes(&a));
  ASSERT_EQ(0, byte_diff(array_start(&a), 11, "hello there"));

  array_reset(&a);
}

/*
 * array_failed / array_empty / array_unallocated
 */
TEST(test_array_macros) {
  array a = ARRAY_INIT();

  ASSERT_EQ(1, array_empty(&a));
  ASSERT_EQ(1, array_unallocated(&a));
  ASSERT_EQ(0, array_failed(&a));

  array_cats(&a, "x");
  ASSERT_EQ(0, array_empty(&a));

  array_reset(&a);
}

#define RUN_ARRAY_TESTS() \
  RUN(test_array_allocate); \
  RUN(test_array_get); \
  RUN(test_array_start); \
  RUN(test_array_end); \
  RUN(test_array_length); \
  RUN(test_array_bytes); \
  RUN(test_array_truncate); \
  RUN(test_array_trunc); \
  RUN(test_array_reset); \
  RUN(test_array_fail); \
  RUN(test_array_equal); \
  RUN(test_array_cat); \
  RUN(test_array_catb); \
  RUN(test_array_cats); \
  RUN(test_array_cats0); \
  RUN(test_array_cat0); \
  RUN(test_array_cate); \
  RUN(test_array_find); \
  RUN(test_array_indexof); \
  RUN(test_array_shift); \
  RUN(test_array_chop); \
  RUN(test_array_splice); \
  RUN(test_array_macros)

TESTS(array) { RUN_ARRAY_TESTS(); }
