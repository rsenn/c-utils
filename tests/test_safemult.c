#include "unit_test.h"
#include "../lib/safemult.h"

/*
 * int umult16(uint16 a, uint16 b, uint16* c);
 * int imult16(int16 a, int16 b, int16* c);
 */
TEST(test_safemult_16) {
  uint16 u;
  int16 s;

  ASSERT_NE(0, umult16(300, 200, &u));
  ASSERT_EQ(60000, u);
  ASSERT_EQ(0, umult16(0xffff, 2, &u));

  ASSERT_NE(0, imult16(100, 300, &s));
  ASSERT_EQ(30000, s);
  ASSERT_EQ(0, imult16(1000, 1000, &s));
}

/*
 * int umult32(uint32 a, uint32 b, uint32* c);
 * int imult32(int32 a, int32 b, int32* c);
 */
TEST(test_safemult_32) {
  uint32 u;
  int32 s;

  ASSERT_NE(0, umult32(1000, 1000, &u));
  ASSERT_EQ(1000000, u);
  ASSERT_EQ(0, umult32(0xffffffffU, 2, &u));

  ASSERT_NE(0, imult32(1000, 1000, &s));
  ASSERT_EQ(1000000, s);
  ASSERT_EQ(0, imult32(1 << 30, 4, &s));
}

/*
 * int umult64(uint64 a, uint64 b, uint64* c);
 * int imult64(int64 a, int64 b, int64* c);
 */
TEST(test_safemult_64) {
  uint64 u;
  int64 s;

  ASSERT_NE(0, umult64(1000000, 1000000, &u));
  ASSERT_EQ(1000000000000ULL, u);
  ASSERT_EQ(0, umult64(0xffffffffffffffffULL, 2, &u));

  ASSERT_NE(0, imult64(1000000, 1000000, &s));
  ASSERT_EQ(1000000000000LL, s);
  ASSERT_EQ(0, imult64(1LL << 62, 4, &s));
}

#define RUN_SAFEMULT_TESTS() \
  RUN(test_safemult_16); \
  RUN(test_safemult_32); \
  RUN(test_safemult_64)

TESTS(safemult) { RUN_SAFEMULT_TESTS(); }
