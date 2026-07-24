#include "unit_test.h"
#include "../lib/str.h"

#include <ctype.h>

/*
 * size_t str_copy(char* out, const char* in);
 * size_t str_copyn(void* p1, const void* p2, size_t max);
 */
TEST(test_str_copy) {
  char buf[8] = {0};

  ASSERT_EQ(5, str_copy(buf, "hello"));
  ASSERT_EQ(0, str_diff(buf, "hello"));

  ASSERT_EQ(4, str_copyn(buf, "abcdef", 4));
}

/*
 * int str_diff(const char* a, const char* b);
 */
TEST(test_str_diff) {
  ASSERT_EQ(0, str_diff("abc", "abc"));
  ASSERT_NE(0, str_diff("abc", "abd"));
}

/*
 * int str_diffn(const char* a, const char* b, size_t limit);
 */
TEST(test_str_diffn) {
  ASSERT_EQ(0, str_diffn("abcXX", "abcYY", 3));
  ASSERT_NE(0, str_diffn("abc", "abd", 3));
}

/*
 * size_t str_len(const char* in);
 */
TEST(test_str_len) {
  ASSERT_EQ(5, str_len("hello"));
  ASSERT_EQ(0, str_len(""));
}

/*
 * size_t str_chr(const char* haystack, char needle);
 * size_t str_chrs(const char* in, const char needles[], size_t nn);
 */
TEST(test_str_chr) {
  ASSERT_EQ(2, str_chr("abcabc", 'c'));
  ASSERT_EQ(6, str_chr("abcabc", 'z'));

  ASSERT_EQ(2, str_chrs("abcabc", "xyc", 3));
}

/*
 * size_t str_str(const char* haystack, const char* needle);
 *
 * Not tested: declared in str.h but has no implementation anywhere in the
 * tree (would fail to link). See BUGS.
 */
TEST(test_str_str) {}

/*
 * size_t str_rchr(const char* haystack, char needle);
 * size_t str_rchrs(const char* haystack, const char needles[], size_t);
 */
TEST(test_str_rchr) {
  ASSERT_EQ(3, str_rchr("abcabc", 'a'));
  ASSERT_EQ(6, str_rchr("abcabc", 'z'));

  ASSERT_EQ(4, str_rchrs("abcabc", "xb", 2));
}

/*
 * int str_endb(const char*, const char*, size_t);
 * int str_end(const char*, const char*);
 */
TEST(test_str_end) {
  ASSERT_NE(0, str_endb("foobar", "bar", 3));
  ASSERT_EQ(0, str_endb("foobar", "foo", 3));

  ASSERT_NE(0, str_end("foobar", "bar"));
  ASSERT_EQ(0, str_end("foobar", "foo"));
}

/*
 * int str_startb(const char*, const char*, size_t);
 * int str_start(const char*, const char*);
 */
TEST(test_str_start) {
  ASSERT_NE(0, str_startb("foobar", "foo", 3));
  ASSERT_EQ(0, str_startb("foobar", "bar", 3));

  ASSERT_NE(0, str_start("foobar", "foo"));
  ASSERT_EQ(0, str_start("foobar", "bar"));
}

/*
 * char* str_istr(char* s, char* find);
 *
 * str_istr only case-folds bytes after the first: it matches *find on the
 * first character case-sensitively, then compares the remainder with
 * str_case_diffn. See BUGS (str-istr-first-char-case-sensitive).
 */
TEST(test_str_istr) {
  char s[] = "Hello World";
  char find_matching_case[] = "World";
  char find_wrong_case[] = "world";

  ASSERT_EQ(0, str_diff(str_istr(s, find_matching_case), "World"));
  ASSERT_EQ(NULL, str_istr(s, find_wrong_case));
}

/*
 * char* str_tok(char* s, const char* delim, char** ptrptr);
 */
TEST(test_str_tok) {
  char s[] = "a,b,c";
  char* ptr;
  char* tok = str_tok(s, ",", &ptr);

  ASSERT_EQ(0, str_diff(tok, "a"));
  tok = str_tok(NULL, ",", &ptr);
  ASSERT_EQ(0, str_diff(tok, "b"));
  tok = str_tok(NULL, ",", &ptr);
  ASSERT_EQ(0, str_diff(tok, "c"));
}

/*
 * char* str_dup(const char* s);
 * char* str_ndup(const char* s, size_t n);
 */
TEST(test_str_dup) {
  char* d = str_dup("hello");

  ASSERT_EQ(0, str_diff(d, "hello"));
  free(d);

  char* n = str_ndup("hello world", 5);
  ASSERT_EQ(0, str_diff(n, "hello"));
  free(n);
}

/*
 * char* str_basename(const char* s);
 */
TEST(test_str_basename) {
  ASSERT_EQ(0, str_diff(str_basename("/a/b/c"), "c"));
  ASSERT_EQ(0, str_diff(str_basename("c"), "c"));
}

/*
 * char* str_decamelize(const char* s, char sep);
 *
 * Not tested: declared in str.h but has no implementation anywhere in the
 * tree (would fail to link). See BUGS.
 */
TEST(test_str_decamelize) {}

/*
 * int str_case_diff(const void* p1, const void* p2);
 * int str_case_diffn(const void* p1, const void* p2, size_t n);
 */
TEST(test_str_case_diff) {
  ASSERT_EQ(0, str_case_diff("ABC", "abc"));
  ASSERT_NE(0, str_case_diff("ABD", "abc"));

  ASSERT_EQ(0, str_case_diffn("ABCxx", "abcyy", 3));
}

/*
 * size_t str_case_equal(const void* s, const void* t);
 */
TEST(test_str_case_equal) {
  ASSERT_NE(0, str_case_equal("ABC", "abc"));
  ASSERT_EQ(0, str_case_equal("ABD", "abc"));
}

/*
 * size_t str_case_start(const void* p1, const void* p2);
 */
TEST(test_str_case_start) {
  ASSERT_NE(0, str_case_start("FOOBAR", "foo"));
  ASSERT_EQ(0, str_case_start("FOOBAR", "bar"));
}

/*
 * size_t str_cat(void* p1, const void* p2);
 * size_t str_catn(void* p1, const void* p2, size_t max);
 */
TEST(test_str_cat) {
  char buf[16];

  str_copy(buf, "foo");
  ASSERT_EQ(6, str_cat(buf, "bar"));
  ASSERT_EQ(0, str_diff(buf, "foobar"));

  /* max caps the *total* resulting length (existing prefix included), not
   * how many bytes of p2 get appended. */
  str_copy(buf, "foo");
  ASSERT_EQ(6, str_catn(buf, "barbaz", 6));
  ASSERT_EQ(0, str_diff(buf, "foobar"));
}

/*
 * size_t str_copyb(void* p1, const void* p2, size_t max);
 */
TEST(test_str_copyb) {
  char buf[8] = {0};

  ASSERT_EQ(3, str_copyb(buf, "abc", 8));
  ASSERT_EQ(0, str_diff(buf, "abc"));
}

/*
 * size_t str_find(const void* s, const void* what);
 * size_t str_findb(const char*, const char* x, size_t n);
 * size_t str_rfindb(const char* s1, const char* x, size_t n);
 * size_t str_rfind(const void* s, const void* what);
 */
TEST(test_str_find) {
  ASSERT_EQ(3, str_find("foobarbaz", "bar"));
  ASSERT_EQ(9, str_find("foobarbaz", "qux"));

  ASSERT_EQ(3, str_findb("foobarbaz", "bar", 3));

  ASSERT_EQ(3, str_rfindb("foobarbaz", "bar", 3));
  ASSERT_EQ(3, str_rfind("foobarbaz", "bar"));
}

/*
 * void str_lower(void* str);
 */
TEST(test_str_lower) {
  char buf[8];

  str_copy(buf, "ABCd");
  str_lower(buf);
  ASSERT_EQ(0, str_diff(buf, "abcd"));
}

/*
 * char* str_ptime(const char* s, const char* format, struct tm* tm);
 */
TEST(test_str_ptime) {
  struct tm tm = {0};
  char* r = str_ptime("2024-01-15", "%Y-%m-%d", &tm);

  ASSERT_NE(NULL, r);
  ASSERT_EQ(2024 - 1900, tm.tm_year);
  ASSERT_EQ(0, tm.tm_mon);
  ASSERT_EQ(15, tm.tm_mday);
}

/*
 * int str_is(const char* s, int (*pred)(int));
 */
TEST(test_str_is) {
  ASSERT_NE(0, str_is("12345", isdigit));
  ASSERT_EQ(0, str_is("123a5", isdigit));
}

/*
 * size_t str_camelize(char*);
 */
TEST(test_str_camelize) {
  char buf[16];

  str_copy(buf, "foo-bar");
  str_camelize(buf);
  ASSERT_EQ(0, str_diff(buf, "fooBar"));
}

/*
 * char* str_triml(char*, const char* charset, unsigned int charsetlen);
 *
 * str_triml's loop condition is inverted relative to byte_triml (its
 * sibling in lib/byte): it stops as soon as it sees a character that IS
 * in charset, instead of skipping over such characters, so it never
 * trims anything whose first character is in charset. See BUGS
 * (str-triml-inverted-condition).
 */
TEST(test_str_triml) {
  char buf[16];

  str_copy(buf, "  foo");
  ASSERT_EQ(0, str_diff(str_triml(buf, " ", 1), "  foo"));
}

/*
 * str_equal, str_has, str_contains macros
 */
TEST(test_str_macros) {
  ASSERT_NE(0, str_equal("abc", "abc"));
  ASSERT_EQ(0, str_equal("abc", "abd"));

  ASSERT_NE(0, str_has("abcdef", 'c'));
  ASSERT_EQ(0, str_has("abcdef", 'z'));

  ASSERT_NE(0, str_contains("abcdef", "cde"));
  ASSERT_EQ(0, str_contains("abcdef", "xyz"));
}

/*
 * size_t str_ansilen(const char* in);
 */
TEST(test_str_ansilen) {
  ASSERT_EQ(3, str_ansilen("abc"));
  ASSERT_EQ(3, str_ansilen("\x1b[31mabc\x1b[0m"));
}

#define RUN_STR_TESTS() \
  RUN(test_str_copy); \
  RUN(test_str_diff); \
  RUN(test_str_diffn); \
  RUN(test_str_len); \
  RUN(test_str_chr); \
  RUN(test_str_str); \
  RUN(test_str_rchr); \
  RUN(test_str_end); \
  RUN(test_str_start); \
  RUN(test_str_istr); \
  RUN(test_str_tok); \
  RUN(test_str_dup); \
  RUN(test_str_basename); \
  RUN(test_str_decamelize); \
  RUN(test_str_case_diff); \
  RUN(test_str_case_equal); \
  RUN(test_str_case_start); \
  RUN(test_str_cat); \
  RUN(test_str_copyb); \
  RUN(test_str_find); \
  RUN(test_str_lower); \
  RUN(test_str_ptime); \
  RUN(test_str_is); \
  RUN(test_str_camelize); \
  RUN(test_str_triml); \
  RUN(test_str_macros); \
  RUN(test_str_ansilen)

TESTS(str) { RUN_STR_TESTS(); }
