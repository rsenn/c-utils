#include "unit_test.h"
#include "../lib/tai.h"

/*
 * void tai_now(struct tai*);
 */
TEST(test_tai_now) {
  struct tai t;

  tai_now(&t);
  ASSERT_NE(0, t.x);
}

/*
 * void tai_add(struct tai*, const struct tai*, const struct tai*);
 * void tai_sub(struct tai*, const struct tai*, const struct tai*);
 */
TEST(test_tai_add_sub) {
  struct tai a, b, sum, diff;

  a.x = 10;
  b.x = 5;

  tai_add(&sum, &a, &b);
  ASSERT_EQ(15, sum.x);

  tai_sub(&diff, &sum, &b);
  ASSERT_EQ(10, diff.x);
}

/*
 * tai_less macro
 */
TEST(test_tai_less) {
  struct tai a, b;

  a.x = 1;
  b.x = 2;

  ASSERT_NE(0, tai_less(&a, &b));
  ASSERT_EQ(0, tai_less(&b, &a));
}

/*
 * tai_approx macro
 */
TEST(test_tai_approx) {
  struct tai t;

  t.x = 42;
  ASSERT_EQ(42.0, tai_approx(&t));
}

/*
 * void tai_pack(char*, const struct tai*);
 * void tai_unpack(const char*, struct tai*);
 */
TEST(test_tai_pack_unpack) {
  struct tai a, b;
  char buf[TAI_PACK];

  a.x = 0x1122334455667788ULL;
  tai_pack(buf, &a);
  tai_unpack(buf, &b);

  ASSERT_EQ(a.x, b.x);
}

/*
 * void tai_uint(struct tai*, unsigned int);
 */
TEST(test_tai_uint) {
  struct tai t;

  tai_uint(&t, 100);
  ASSERT_EQ(100, t.x);
}

/*
 * tai_unix macro
 */
TEST(test_tai_unix) {
  struct tai t;

  tai_unix(&t, 0);
  ASSERT_EQ((uint64)4611686018427387914ULL, t.x);
}

#define RUN_TAI_TESTS() \
  RUN(test_tai_now); \
  RUN(test_tai_add_sub); \
  RUN(test_tai_less); \
  RUN(test_tai_approx); \
  RUN(test_tai_pack_unpack); \
  RUN(test_tai_uint); \
  RUN(test_tai_unix)

TESTS(tai) { RUN_TAI_TESTS(); }
