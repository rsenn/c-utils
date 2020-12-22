#include "unit_test.h"
#include "../lib/iarray.h"

/*
 * void iarray_init(iarray* sa);
 */
TEST(test_iarray_init) {
  iarray a;
  iarray_init(&a, sizeof(ssize_t));

  ASSERT_EQ(0, a.len);
}

/*
 * iarray_allocate
 */
TEST(test_iarray_allocate) {
  iarray a;
  iarray_init(&a, sizeof(ssize_t));

  *(size_t*)iarray_allocate(&a, 15) = 16;

  ASSERT_EQ(16, *(size_t*)iarray_get(&a, 15));
}

/*
 * size_t iarray_length
 */
TEST(test_iarray_length) {
  iarray a;
  ssize_t* c;
  iarray_init(&a, sizeof(ssize_t));

  c = iarray_allocate(&a, 10);
  ASSERT_EQ(11, iarray_length(&a));
}

/*
 * void* iarray_get
 */
TEST(test_iarray_get) {
  iarray a;
  ssize_t* c;
  iarray_init(&a, sizeof(ssize_t));

  *(size_t*)iarray_allocate(&a, 0) = 1;
  *(size_t*)iarray_allocate(&a, 1) = 2;
  *(size_t*)iarray_allocate(&a, 2) = 3;
  *(size_t*)iarray_allocate(&a, 3) = 4;
  *(size_t*)iarray_allocate(&a, 4) = 5;

  ASSERT_EQ(1, *(size_t*)iarray_get(&a, 0));
  ASSERT_EQ(2, *(size_t*)iarray_get(&a, 1));
  ASSERT_EQ(3, *(size_t*)iarray_get(&a, 2));
  ASSERT_EQ(4, *(size_t*)iarray_get(&a, 3));
  ASSERT_EQ(5, *(size_t*)iarray_get(&a, 4));
}

#define RUN_STRALLOC_TESTS()                                                                                                                                                                           \
  RUN(test_iarray_init);                                                                                                                                                                               \
  RUN(test_iarray_allocate);                                                                                                                                                                           \
  RUN(test_iarray_length);                                                                                                                                                                             \
  RUN(test_iarray_get);

TESTS(iarray) { RUN_STRALLOC_TESTS(); }
