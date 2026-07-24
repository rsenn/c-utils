#include "unit_test.h"
#include "../lib/case.h"
#include "../lib/byte.h"
#include "../lib/str.h"

/*
 * void case_lowers(char* s);
 */
TEST(test_case_lowers) {
  char buf[8];

  str_copy(buf, "AbCdEf");
  case_lowers(buf);
  ASSERT_EQ(0, str_diff(buf, "abcdef"));
}

/*
 * void case_lowerb(void* buf, size_t len);
 */
TEST(test_case_lowerb) {
  char buf[6];

  byte_copy(buf, 6, "ABCDEF");
  case_lowerb(buf, 6);
  ASSERT_EQ(0, byte_diff(buf, 6, "abcdef"));
}

/*
 * int case_lowerc(int c);
 */
TEST(test_case_lowerc) {
  ASSERT_EQ('a', case_lowerc('A'));
  ASSERT_EQ('a', case_lowerc('a'));
  ASSERT_EQ('.', case_lowerc('.'));
}

/*
 * int case_diffs(const char*, const char*);
 */
TEST(test_case_diffs) {
  ASSERT_EQ(0, case_diffs("Hello", "HELLO"));
  ASSERT_NE(0, case_diffs("Hello", "World"));
}

/*
 * int case_diffb(const void*, size_t, const void*);
 */
TEST(test_case_diffb) {
  ASSERT_EQ(0, case_diffb("Hello", 5, "HELLO"));
  ASSERT_NE(0, case_diffb("Hello", 5, "World"));
}

/*
 * int case_starts(const char*, const char*);
 */
TEST(test_case_starts) {
  ASSERT_NE(0, case_starts("HelloWorld", "hello"));
  ASSERT_EQ(0, case_starts("HelloWorld", "world"));
}

/*
 * #define case_equals(s, t)
 */
TEST(test_case_equals) {
  ASSERT_NE(0, case_equals("Hello", "HELLO"));
  ASSERT_EQ(0, case_equals("Hello", "World"));
}

/*
 * #define case_equalb(s, n, t)
 */
TEST(test_case_equalb) {
  ASSERT_NE(0, case_equalb("Hello", 5, "HELLO"));
  ASSERT_EQ(0, case_equalb("Hello", 5, "World"));
}

/*
 * size_t case_findb(const void*, size_t, const void*, size_t);
 */
TEST(test_case_findb) {
  ASSERT_EQ(3, case_findb("fooBARbaz", 9, "bar", 3));
  ASSERT_EQ(9, case_findb("fooBARbaz", 9, "qux", 3));
}

/*
 * size_t case_finds(const void*, size_t, const char*);
 */
TEST(test_case_finds) {
  ASSERT_EQ(3, case_finds("fooBARbaz", 9, "bar"));
  ASSERT_EQ(9, case_finds("fooBARbaz", 9, "qux"));
}

/*
 * size_t case_find(const void*, const void*);
 */
TEST(test_case_find) {
  ASSERT_EQ(3, case_find("fooBARbaz", "bar"));
  ASSERT_EQ(9, case_find("fooBARbaz", "qux"));
}

#define RUN_CASE_TESTS() \
  RUN(test_case_lowers); \
  RUN(test_case_lowerb); \
  RUN(test_case_lowerc); \
  RUN(test_case_diffs); \
  RUN(test_case_diffb); \
  RUN(test_case_starts); \
  RUN(test_case_equals); \
  RUN(test_case_equalb); \
  RUN(test_case_findb); \
  RUN(test_case_finds); \
  RUN(test_case_find)

TESTS(case) { RUN_CASE_TESTS(); }
