#include "unit_test.h"
#include "../lib/textcode.h"
#include "../lib/byte.h"

/*
 * size_t fmt_jsonescape(char* dest, const char* src, size_t len);
 */
TEST(test_fmt_jsonescape) {
  char buf[64];

  ASSERT_EQ(10, fmt_jsonescape(buf, "foo\nbar\\", 8));
  ASSERT_EQ(1, byte_equal(buf, 10, "foo\\nbar\\\\"));
}

/*
 * size_t scan_jsonescape(const char* src, char* dest, size_t* destlen);
 */
TEST(test_scan_jsonescape) {
  char buf[64];
  size_t l;

  byte_zero(buf, sizeof(buf));
  ASSERT_EQ(14, scan_jsonescape("foo\\u000abar\\\\", buf, &l));
  ASSERT_EQ(8, l);
  ASSERT_EQ(1, byte_equal(buf, 8, "foo\nbar\\"));
}

/*
 * example from the json spec: G clef U+1D11E encoded as UTF-16 surrogates
 */
TEST(test_scan_jsonescape_surrogates) {
  char buf[64];
  size_t l;

  byte_zero(buf, sizeof(buf));
  ASSERT_EQ(15, scan_jsonescape("\\uD834\\uDD1Exyz", buf, &l));
  ASSERT_EQ(7, l);
  ASSERT_EQ(1, byte_equal(buf, 7, "\xf0\x9d\x84\x9exyz"));
}

#define RUN_TEXTCODE_TESTS() \
  RUN(test_fmt_jsonescape); \
  RUN(test_scan_jsonescape); \
  RUN(test_scan_jsonescape_surrogates)

TESTS(textcode) { RUN_TEXTCODE_TESTS(); }
