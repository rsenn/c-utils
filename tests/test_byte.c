#include "unit_test.h"
#include "../lib/byte.h"

/*
 * size_t byte_chr(const void* haystack, size_t len, char needle);
 */
TEST(test_byte_chr) {
  ASSERT_EQ(2, byte_chr("abcabc", 6, 'c'));
  ASSERT_EQ(6, byte_chr("abcabc", 6, 'z'));
}

/*
 * size_t byte_chrs(const char*, size_t, const char charset[], size_t setlen);
 */
TEST(test_byte_chrs) {
  ASSERT_EQ(2, byte_chrs("abcabc", 6, "xyzc", 4));
  ASSERT_EQ(6, byte_chrs("abcabc", 6, "xyz", 3));
}

/*
 * size_t byte_chrns(const char*, size_t, const char charset[], size_t setlen);
 */
TEST(test_byte_chrns) { ASSERT_EQ(4, byte_chrns("aabbc", 5, "ab", 2)); }

/*
 * size_t byte_rchr(const void* haystack, size_t len, char needle);
 */
TEST(test_byte_rchr) {
  ASSERT_EQ(3, byte_rchr("abcabc", 6, 'a'));
  ASSERT_EQ(6, byte_rchr("abcabc", 6, 'z'));
}

/*
 * size_t byte_rchrs(const void*, size_t, const char*, size_t);
 */
TEST(test_byte_rchrs) { ASSERT_EQ(4, byte_rchrs("abcabc", 6, "xb", 2)); }

/*
 * void byte_copy(void* out, size_t len, const void* in);
 */
TEST(test_byte_copy) {
  char buf[8] = {0};

  byte_copy(buf, 5, "hello");
  ASSERT_EQ(0, byte_diff(buf, 5, "hello"));
}

/*
 * void byte_copyr(void* out, size_t len, const void* in);
 */
TEST(test_byte_copyr) {
  char buf[6] = {0};

  byte_copyr(buf, 5, "hello");
  ASSERT_EQ(0, byte_diff(buf, 5, "hello"));
}

/*
 * int byte_diff(const void* a, size_t len, const void* b);
 */
TEST(test_byte_diff) {
  ASSERT_EQ(0, byte_diff("abc", 3, "abc"));
  ASSERT_NE(0, byte_diff("abd", 3, "abc"));
}

/*
 * void byte_zero(void* out, size_t len);
 */
TEST(test_byte_zero) {
  char buf[4] = {1, 2, 3, 4};

  byte_zero(buf, 4);
  ASSERT_EQ(0, buf[0]);
  ASSERT_EQ(0, buf[1]);
  ASSERT_EQ(0, buf[2]);
  ASSERT_EQ(0, buf[3]);
}

/*
 * int byte_equal_notimingattack(const void* a, size_t len, const void* b);
 *
 * Not tested: declared in byte.h but has no implementation anywhere in
 * the tree (would fail to link). See BUGS.
 */
TEST(test_byte_equal_notimingattack) {}

/*
 * void byte_fill(void* out, size_t len, int c);
 */
TEST(test_byte_fill) {
  char buf[4];

  byte_fill(buf, 4, 'x');
  ASSERT_EQ(0, byte_diff(buf, 4, "xxxx"));
}

/*
 * int byte_case_diff(const void* x1, size_t len, const void* x2);
 */
TEST(test_byte_case_diff) {
  ASSERT_EQ(0, byte_case_diff("ABC", 3, "abc"));
  ASSERT_NE(0, byte_case_diff("ABD", 3, "abc"));
}

/*
 * size_t byte_case_equal(const void* s, size_t len, const void* t);
 */
TEST(test_byte_case_equal) {
  ASSERT_NE(0, byte_case_equal("ABC", 3, "abc"));
  ASSERT_EQ(0, byte_case_equal("ABD", 3, "abc"));
}

/*
 * size_t byte_count(const void* s, size_t n, char c);
 */
TEST(test_byte_count) { ASSERT_EQ(3, byte_count("aXaXaX", 6, 'X')); }

/*
 * size_t byte_equal(const void* s, size_t n, const void* t);
 */
TEST(test_byte_equal) {
  ASSERT_NE(0, byte_equal("abc", 3, "abc"));
  ASSERT_EQ(0, byte_equal("abc", 3, "abd"));
}

/*
 * void byte_lower(void* s, size_t len);
 */
TEST(test_byte_lower) {
  char buf[4];

  byte_copy(buf, 4, "ABCd");
  byte_lower(buf, 4);
  ASSERT_EQ(0, byte_diff(buf, 4, "abcd"));
}

/*
 * void byte_upper(void* s, size_t len);
 */
TEST(test_byte_upper) {
  char buf[4];

  byte_copy(buf, 4, "abcD");
  byte_upper(buf, 4);
  ASSERT_EQ(0, byte_diff(buf, 4, "ABCD"));
}

/*
 * size_t byte_column(const void* s, size_t n);
 */
TEST(test_byte_column) {
  ASSERT_EQ(4, byte_column("foo\nbar", 7));
  ASSERT_EQ(3, byte_column("abc", 3));
}

/*
 * size_t byte_findb(const void* haystack, size_t hlen, const void* what, size_t wlen);
 */
TEST(test_byte_findb) {
  ASSERT_EQ(3, byte_findb("foobarbaz", 9, "bar", 3));
  ASSERT_EQ(9, byte_findb("foobarbaz", 9, "qux", 3));
}

/*
 * size_t byte_finds(const void* haystack, size_t hlen, const char* what);
 */
TEST(test_byte_finds) {
  ASSERT_EQ(3, byte_finds("foobarbaz", 9, "bar"));
  ASSERT_EQ(9, byte_finds("foobarbaz", 9, "qux"));
}

/*
 * void byte_replace(char* x, size_t n, char before, char after);
 */
TEST(test_byte_replace) {
  char buf[6];

  byte_copy(buf, 6, "a-b-c");
  buf[5] = 0;
  byte_replace(buf, 5, '-', '_');
  ASSERT_EQ(0, byte_diff(buf, 5, "a_b_c"));
}

/*
 * size_t byte_ccopy(void* dst, size_t count, const void* src, char c);
 */
TEST(test_byte_ccopy) {
  char buf[8] = {0};

  ASSERT_EQ(4, byte_ccopy(buf, 8, "abc\0def", '\0'));
  ASSERT_EQ(0, byte_diff(buf, 3, "abc"));
}

/*
 * const char* byte_triml(const char* x, size_t* len, const char* charset, unsigned int charsetlen);
 */
TEST(test_byte_triml) {
  size_t len = 6;
  const char* r = byte_triml("  foo ", &len, " ", 1);

  ASSERT_EQ(0, byte_diff(r, 4, "foo "));
  ASSERT_EQ(4, len);
}

/*
 * size_t byte_trimr(const char* x, size_t n, const char* trimchars, unsigned int trimcharslen);
 */
TEST(test_byte_trimr) { ASSERT_EQ(3, byte_trimr("foo  ", 5, " ", 1)); }

/*
 * char* byte_trim(char* x, size_t* n, const char* trimchars, unsigned int trimcharslen);
 */
TEST(test_byte_trim) {
  char buf[8];
  size_t n = 7;

  byte_copy(buf, 7, "  foo  ");

  char* r = byte_trim(buf, &n, " ", 1);

  ASSERT_EQ(3, n);
  ASSERT_EQ(0, byte_diff(r, 3, "foo"));
}

/*
 * size_t byte_camelize(char* x, size_t len);
 */
TEST(test_byte_camelize) {}

/*
 * int byte_startb(const void*, size_t, const char*, size_t);
 */
TEST(test_byte_startb) {
  ASSERT_NE(0, byte_startb("foobar", 6, "foo", 3));
  ASSERT_EQ(0, byte_startb("foobar", 6, "bar", 3));
}

/*
 * int byte_starts(const void*, size_t, const char*);
 */
TEST(test_byte_starts) {
  ASSERT_NE(0, byte_starts("foobar", 6, "foo"));
  ASSERT_EQ(0, byte_starts("foobar", 6, "bar"));
}

/*
 * int byte_endb(const void*, size_t, const char*, size_t);
 */
TEST(test_byte_endb) {
  ASSERT_NE(0, byte_endb("foobar", 6, "bar", 3));
  ASSERT_EQ(0, byte_endb("foobar", 6, "foo", 3));
}

/*
 * int byte_ends(const void*, size_t, const char*);
 */
TEST(test_byte_ends) {
  ASSERT_NE(0, byte_ends("foobar", 6, "bar"));
  ASSERT_EQ(0, byte_ends("foobar", 6, "foo"));
}

/*
 * int byte_diff2(const char* a, size_t alen, const char* b, size_t blen);
 */
TEST(test_byte_diff2) {
  ASSERT_EQ(0, byte_diff2("abc", 3, "abc", 3));
  ASSERT_NE(0, byte_diff2("abc", 3, "abcd", 4));
}

#define RUN_BYTE_TESTS() \
  RUN(test_byte_chr); \
  RUN(test_byte_chrs); \
  RUN(test_byte_chrns); \
  RUN(test_byte_rchr); \
  RUN(test_byte_rchrs); \
  RUN(test_byte_copy); \
  RUN(test_byte_copyr); \
  RUN(test_byte_diff); \
  RUN(test_byte_zero); \
  RUN(test_byte_equal_notimingattack); \
  RUN(test_byte_fill); \
  RUN(test_byte_case_diff); \
  RUN(test_byte_case_equal); \
  RUN(test_byte_count); \
  RUN(test_byte_equal); \
  RUN(test_byte_lower); \
  RUN(test_byte_upper); \
  RUN(test_byte_column); \
  RUN(test_byte_findb); \
  RUN(test_byte_finds); \
  RUN(test_byte_replace); \
  RUN(test_byte_ccopy); \
  RUN(test_byte_triml); \
  RUN(test_byte_trimr); \
  RUN(test_byte_trim); \
  RUN(test_byte_camelize); \
  RUN(test_byte_startb); \
  RUN(test_byte_starts); \
  RUN(test_byte_endb); \
  RUN(test_byte_ends); \
  RUN(test_byte_diff2)

TESTS(byte) { RUN_BYTE_TESTS(); }
