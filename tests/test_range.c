#include "unit_test.h"
#include "../lib/range.h"

/*
 * void range_init(range*, void*, size_t, size_t);
 * size_t range_size(const range*);
 * int range_empty(const range*);
 */
TEST(test_range_init) {
  int a[5] = {10, 20, 30, 40, 50};
  range r;

  range_init(&r, a, 5, sizeof(int));
  ASSERT_EQ(5, range_size(&r));
  ASSERT_EQ(0, range_empty(&r));
  ASSERT_EQ(sizeof(int), r.elem_size);
  ASSERT_EQ((void*)a, range_start(&r));
  ASSERT_EQ((void*)(a + 5), range_end(&r));

  range_init(&r, a, 0, sizeof(int));
  ASSERT_EQ(0, range_size(&r));
  ASSERT_NE(0, range_empty(&r));

  /* an element size of 0 is taken as 1 */
  range_init(&r, a, 3, 0);
  ASSERT_EQ(1, r.elem_size);
  ASSERT_EQ(0, range_size(&r));
}

/*
 * void* range_index(range*, size_t);
 * void* range_next(range*, void*);
 * void* range_plus(range*, void*, size_t);
 * void range_add(range*, void**, size_t);
 * range_foreach
 */
TEST(test_range_iteration) {
  int a[4] = {1, 2, 3, 4};
  range r;
  int* p;
  int sum = 0;
  void* q;

  range_init(&r, a, 4, sizeof(int));

  ASSERT_EQ(3, *(int*)range_index(&r, 2));
  ASSERT_EQ(2, *(int*)range_next(&r, a));
  ASSERT_EQ(4, *(int*)range_plus(&r, a, 3));

  q = a;
  range_add(&r, &q, 2);
  ASSERT_EQ(3, *(int*)q);

  range_foreach(&r, p) sum += *p;
  ASSERT_EQ(10, sum);
}

/*
 * void range_rotate(range*, ssize_t);
 */
TEST(test_range_rotate) {
  int a[5] = {1, 2, 3, 4, 5};
  range r;

  range_init(&r, a, 5, sizeof(int));

  /* left by 2 */
  range_rotate(&r, 2);
  ASSERT_EQ(3, a[0]);
  ASSERT_EQ(4, a[1]);
  ASSERT_EQ(5, a[2]);
  ASSERT_EQ(1, a[3]);
  ASSERT_EQ(2, a[4]);

  /* a negative count rotates the other way: back to the start */
  range_rotate(&r, -2);
  ASSERT_EQ(1, a[0]);
  ASSERT_EQ(2, a[1]);
  ASSERT_EQ(3, a[2]);
  ASSERT_EQ(4, a[3]);
  ASSERT_EQ(5, a[4]);

  /* 0 and the full length change nothing */
  range_rotate(&r, 0);
  range_rotate(&r, 5);
  ASSERT_EQ(1, a[0]);
  ASSERT_EQ(5, a[4]);

  /* one step */
  range_rotate(&r, 1);
  ASSERT_EQ(2, a[0]);
  ASSERT_EQ(1, a[4]);
}

TEST(test_range_rotate_bytes) {
  char s[] = "abcdef";
  range r;

  range_init(&r, s, 6, 1);
  range_rotate(&r, 4);
  ASSERT_STR_EQUAL("efabcd", s);
}

#define RUN_RANGE_TESTS() \
  RUN(test_range_init); \
  RUN(test_range_iteration); \
  RUN(test_range_rotate); \
  RUN(test_range_rotate_bytes)

TESTS(range) { RUN_RANGE_TESTS(); }
