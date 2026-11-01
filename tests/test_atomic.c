#include "unit_test.h"
#include "../lib/atomic.h"

/* libowfat CAS.h compatible atomic word operations */
TEST(test_atomic_ops) {
  size_t x = 5;

  ASSERT_EQ(1, compare_and_swap(&x, 5, 7));
  ASSERT_EQ(7, x);
  ASSERT_EQ(0, compare_and_swap(&x, 5, 9));
  ASSERT_EQ(7, x);
  ASSERT_EQ(10, atomic_add_return(&x, 3));
  atomic_inc(&x);
  ASSERT_EQ(11, x);
  ASSERT_EQ(10, atomic_dec_return(&x));
  ASSERT_EQ(11, atomic_inc_return(&x));
  atomic_dec(&x);
  atomic_or(&x, 5);
  ASSERT_EQ(15, x);
  atomic_and(&x, 6);
  ASSERT_EQ(6, x);
}

TESTS(atomic) { RUN(test_atomic_ops); }
