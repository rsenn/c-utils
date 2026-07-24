#include "unit_test.h"
#include "../lib/uint16.h"

/*
 * void uint16_pack(char* out, uint16 in);
 * void uint16_unpack(const char* in, uint16* out);
 */
TEST(test_uint16_pack_unpack) {
  char buf[2];
  uint16 out;

  uint16_pack(buf, 0x1234);
  uint16_unpack(buf, &out);
  ASSERT_EQ(0x1234, out);
}

/*
 * uint16 uint16_get(const void* ptr);
 * uint16 uint16_read(const char* in);
 */
TEST(test_uint16_get_read) {
  char buf[2];

  uint16_pack(buf, 0xbeef);
  ASSERT_EQ(0xbeef, uint16_get(buf));
  ASSERT_EQ(0xbeef, uint16_read(buf));
}

/*
 * void uint16_pack_big(char* out, uint16 in);
 * void uint16_unpack_big(const char* in, uint16* out);
 * uint16 uint16_read_big(const char*);
 */
TEST(test_uint16_pack_unpack_big) {
  char buf[2];
  uint16 out;

  uint16_pack_big(buf, 0x1234);
  ASSERT_EQ((unsigned char)0x12, (unsigned char)buf[0]);
  ASSERT_EQ((unsigned char)0x34, (unsigned char)buf[1]);

  uint16_unpack_big(buf, &out);
  ASSERT_EQ(0x1234, out);
  ASSERT_EQ(0x1234, uint16_read_big(buf));
}

#define RUN_UINT16_TESTS() \
  RUN(test_uint16_pack_unpack); \
  RUN(test_uint16_get_read); \
  RUN(test_uint16_pack_unpack_big)

TESTS(uint16) { RUN_UINT16_TESTS(); }
