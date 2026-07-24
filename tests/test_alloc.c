#include "unit_test.h"
#include "../lib/alloc.h"
#include "../lib/byte.h"

/*
 * void* alloc(size_t n);
 */
TEST(test_alloc_alloc) {
  char* p = alloc(16);

  ASSERT_NE(0, (size_t)p);

  byte_fill(p, 16, 0x5a);
  ASSERT_EQ(0x5a, p[0]);
  ASSERT_EQ(0x5a, p[15]);

  alloc_free(p);
}

/*
 * void* alloc_zero(size_t n);
 */
TEST(test_alloc_zero) {
  unsigned char* p = alloc_zero(32);
  int all_zero = 1;

  ASSERT_NE(0, (size_t)p);

  for(size_t i = 0; i < 32; i++)
    if(p[i] != 0)
      all_zero = 0;

  ASSERT_EQ(1, all_zero);

  alloc_free(p);
}

/*
 * void alloc_free(void* x);
 */
TEST(test_alloc_free) {
  char* p = alloc(8);

  ASSERT_NE(0, (size_t)p);
  alloc_free(p);
}

/*
 * void alloc_clear(void* ptr);
 */
TEST(test_alloc_clear) {}

/*
 * int alloc_re(void* x, size_t m, size_t n);
 */
TEST(test_alloc_re) {
  char* p = alloc(4);

  ASSERT_NE(0, (size_t)p);
  byte_copy(p, 4, "abcd");

  ASSERT_NE(0, alloc_re(&p, 4, 64));
  ASSERT_EQ(0, byte_diff(p, 4, "abcd"));

  alloc_free(p);
}

#define RUN_ALLOC_TESTS() \
  RUN(test_alloc_alloc); \
  RUN(test_alloc_zero); \
  RUN(test_alloc_free); \
  RUN(test_alloc_clear); \
  RUN(test_alloc_re)

TESTS(alloc) { RUN_ALLOC_TESTS(); }
