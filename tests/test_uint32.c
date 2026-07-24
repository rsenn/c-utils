#include "unit_test.h"
#include "../lib/uint32.h"

/*
 * void uint32_pack(char* out, uint32 in);
 * void uint32_unpack(const char* in, uint32* out);
 */
TEST(test_uint32_pack_unpack) {
  char buf[4];
  uint32 out;

  uint32_pack(buf, 0x12345678);
  uint32_unpack(buf, &out);
  ASSERT_EQ(0x12345678, out);
}

/*
 * uint32 uint32_get(const void* ptr);
 * uint32 uint32_read(const char* in);
 */
TEST(test_uint32_get_read) {
  char buf[4];

  uint32_pack(buf, 0xdeadbeef);
  ASSERT_EQ(0xdeadbeef, uint32_get(buf));
  ASSERT_EQ(0xdeadbeef, uint32_read(buf));
}

/*
 * void uint32_pack_big(char* out, uint32 in);
 * void uint32_unpack_big(const char* in, uint32* out);
 * uint32 uint32_read_big(const char* in);
 */
TEST(test_uint32_pack_unpack_big) {
  char buf[4];
  uint32 out;

  uint32_pack_big(buf, 0x12345678);
  ASSERT_EQ((unsigned char)0x12, (unsigned char)buf[0]);
  ASSERT_EQ((unsigned char)0x78, (unsigned char)buf[3]);

  uint32_unpack_big(buf, &out);
  ASSERT_EQ(0x12345678, out);
  ASSERT_EQ(0x12345678, uint32_read_big(buf));
}

/*
 * uint32 uint32_random(void);
 */
TEST(test_uint32_random) {
  uint32 a = uint32_random();
  uint32 b = uint32_random();

  (void)a;
  (void)b;
}

/*
 * int uint32_seed(const void*, unsigned long n);
 * uint32 uint32_prng(uint32, uint32 seed);
 */
TEST(test_uint32_prng) {
  uint32 a = uint32_prng(1, 42);
  uint32 b = uint32_prng(1, 42);

  ASSERT_EQ(a, b);
}

/*
 * uint32_ror, uint32_rol macros
 */
TEST(test_uint32_rotate) {
  ASSERT_EQ(0x00000001, uint32_ror(0x00000002, 1));
  ASSERT_EQ(0x00000002, uint32_rol(0x00000001, 1));
}

#define RUN_UINT32_TESTS() \
  RUN(test_uint32_pack_unpack); \
  RUN(test_uint32_get_read); \
  RUN(test_uint32_pack_unpack_big); \
  RUN(test_uint32_random); \
  RUN(test_uint32_prng); \
  RUN(test_uint32_rotate)

TESTS(uint32) { RUN_UINT32_TESTS(); }
