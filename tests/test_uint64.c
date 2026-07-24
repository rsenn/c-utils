#include "unit_test.h"
#include "../lib/uint64.h"

/*
 * uint64_pack/uint64_unpack (macros or functions depending on platform)
 */
TEST(test_uint64_pack_unpack) {
  char buf[8];
  uint64 out;

  uint64_pack(buf, 0x1234567890abcdefULL);
  uint64_unpack(buf, &out);
  ASSERT_EQ(0x1234567890abcdefULL, out);
}

/*
 * uint64 uint64_get(const void* ptr);
 * uint64_read (macro or function)
 */
TEST(test_uint64_get_read) {
  char buf[8];

  uint64_pack(buf, 0xdeadbeefcafebabeULL);
  ASSERT_EQ(0xdeadbeefcafebabeULL, uint64_get(buf));
  ASSERT_EQ(0xdeadbeefcafebabeULL, uint64_read(buf));
}

/*
 * void uint64_pack_big(char* out, uint64 in);
 * void uint64_unpack_big(const char* in, uint64* out);
 * uint64 uint64_read_big(const char* in);
 */
TEST(test_uint64_pack_unpack_big) {
  char buf[8];
  uint64 out;

  uint64_pack_big(buf, 0x1234567890abcdefULL);
  ASSERT_EQ((unsigned char)0x12, (unsigned char)buf[0]);
  ASSERT_EQ((unsigned char)0xef, (unsigned char)buf[7]);

  uint64_unpack_big(buf, &out);
  ASSERT_EQ(0x1234567890abcdefULL, out);
  ASSERT_EQ(0x1234567890abcdefULL, uint64_read_big(buf));
}

/*
 * uint64_to_double macro
 */
TEST(test_uint64_to_double) {
  ASSERT_EQ(42.0, uint64_to_double((uint64)42));
}

#define RUN_UINT64_TESTS() \
  RUN(test_uint64_pack_unpack); \
  RUN(test_uint64_get_read); \
  RUN(test_uint64_pack_unpack_big); \
  RUN(test_uint64_to_double)

TESTS(uint64) { RUN_UINT64_TESTS(); }
