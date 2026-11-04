#include "../lib/stralloc.h"
#include "unit_test.h"
#include "../lib/stralloc.h"
#include "../lib/byte.h"
#include "../lib/scan.h"
#include "../lib/str.h"
#include "../lib/buffer.h"
#include "../lib/open.h"
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>

/* true when sa holds exactly the NUL-free string s */
static int
is(const stralloc* sa, const char* s) {
  return sa->len == str_len(s) && byte_equal(sa->s, sa->len, s);
}

static size_t
fmt_angle(char* d, int c) {
  if(d) {
    d[0] = '<';
    d[1] = (char)c;
    d[2] = '>';
  }
  return 3;
}

static size_t
scan_nothing(const char* s, char* d) {
  (void)s;
  (void)d;
  return 0;
}

static int
is_upper(int c) {
  return c >= 'A' && c <= 'Z';
}

/*
 * void stralloc_init(stralloc* sa);
 */

TEST(test_stralloc_init) {
  stralloc sa;

  stralloc_init(&sa);

  ASSERT_EQ(0, sa.s);
  ASSERT_EQ(0, sa.len);
  ASSERT_EQ(0, sa.a);
}

/*
 * int stralloc_ready(stralloc* sa, size_t len);
 */

TEST(test_stralloc_ready) {
  stralloc sa;

  stralloc_init(&sa);
  stralloc_ready(&sa, 40);

  ASSERT_GE(sa.a, 40);

  stralloc_free(&sa);
}

/*
 * int stralloc_readyplus(stralloc* sa, size_t len);
 */

TEST(test_stralloc_readyplus) {
  stralloc sa;

  stralloc_init(&sa);
  stralloc_ready(&sa, 40);
  stralloc_readyplus(&sa, 20);

  ASSERT_GE(sa.a, 60);

  stralloc_free(&sa);
}

/*
 * int stralloc_copyb(stralloc* sa, const char* buf, size_t len);
 */

TEST(test_stralloc_copyb) {
  stralloc sa;
  stralloc_init(&sa);
  ASSERT_EQ(1, stralloc_copyb(&sa, "abcdef", 3));
  ASSERT_EQ(1, is(&sa, "abc"));
  ASSERT_EQ(1, stralloc_copyb(&sa, "xy", 2));
  ASSERT_EQ(1, is(&sa, "xy"));
  stralloc_free(&sa);
}

/*
 * int stralloc_copys(stralloc* sa, const char* buf);
 */

TEST(test_stralloc_copys) {
  stralloc sa;
  stralloc_init(&sa);
  ASSERT_EQ(1, stralloc_copys(&sa, "hello"));
  ASSERT_EQ(1, is(&sa, "hello"));
  stralloc_copys(&sa, "");
  ASSERT_EQ(0, sa.len);
  stralloc_free(&sa);
}

/*
 * int stralloc_copy(stralloc* sa, const stralloc* sa2);
 */

TEST(test_stralloc_copy) {
  stralloc a, b;
  stralloc_init(&a);
  stralloc_init(&b);
  stralloc_copys(&b, "XYZ");
  ASSERT_EQ(1, stralloc_copy(&a, &b));
  ASSERT_EQ(1, is(&a, "XYZ"));
  stralloc_free(&a);
  stralloc_free(&b);
}

/*
 * int stralloc_catb(stralloc* sa, const char* in, size_t len);
 */

TEST(test_stralloc_catb) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "XYZ");
  ASSERT_EQ(1, stralloc_catb(&sa, "12345", 2));
  ASSERT_EQ(1, is(&sa, "XYZ12"));
  stralloc_free(&sa);
}

/*
 * int stralloc_cats(stralloc* sa, const char* in);
 */

TEST(test_stralloc_cats) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_cats(&sa, "ab");
  ASSERT_EQ(1, stralloc_cats(&sa, "cd"));
  ASSERT_EQ(1, is(&sa, "abcd"));
  stralloc_free(&sa);
}

/*
 * int stralloc_catc(stralloc* sa, const unsigned char c);
 */

TEST(test_stralloc_catc) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_cats(&sa, "ab");
  ASSERT_EQ(1, stralloc_catc(&sa, '#'));
  ASSERT_EQ(1, is(&sa, "ab#"));
  stralloc_free(&sa);
}

/*
 * void stralloc_zero(stralloc* sa);
 */

TEST(test_stralloc_zero) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "abc");
  stralloc_zero(&sa);
  ASSERT_EQ(0, sa.len);
  ASSERT_EQ(1, sa.a >= 3);
  /* keeps the allocation */ stralloc_free(&sa);
}

/*
 * int stralloc_trunc(stralloc* sa, size_t n);
 */

TEST(test_stralloc_trunc) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "abcdef");
  ASSERT_EQ(1, stralloc_trunc(&sa, 2));
  ASSERT_EQ(1, is(&sa, "ab"));
  stralloc_free(&sa);
}

/*
 * int stralloc_catm_internal(stralloc* sa, ...);
 */

TEST(test_stralloc_catm_internal) {
  stralloc sa;
  stralloc_init(&sa);
  ASSERT_EQ(1, stralloc_catm(&sa, "a", "bc", "d") != 0);
  ASSERT_EQ(1, is(&sa, "abcd"));
  stralloc_copym(&sa, "x", "y");
  ASSERT_EQ(1, is(&sa, "xy"));
  stralloc_free(&sa);
}

/*
 * int stralloc_cat(stralloc* sa, const stralloc* in);
 */

TEST(test_stralloc_cat) {
  stralloc a, b;
  stralloc_init(&a);
  stralloc_init(&b);
  stralloc_copys(&a, "ab");
  stralloc_copys(&b, "XYZ");
  ASSERT_EQ(1, stralloc_cat(&a, &b));
  ASSERT_EQ(1, is(&a, "abXYZ"));
  stralloc_free(&a);
  stralloc_free(&b);
}

/*
 * int stralloc_diffs(const stralloc *a, const char *b);
 */

TEST(test_stralloc_diffs) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "Hello");
  ASSERT_EQ(0, stralloc_diffs(&sa, "Hello"));
  ASSERT_NE(0, stralloc_diffs(&sa, "hello"));
  ASSERT_NE(0, stralloc_diffs(&sa, "Hell"));
  stralloc_free(&sa);
}

/*
 * int stralloc_nul(stralloc* sa);
 */

TEST(test_stralloc_nul) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "abc");
  ASSERT_EQ(1, stralloc_nul(&sa));
  ASSERT_EQ(3, sa.len);
  /* the terminator is not counted */ ASSERT_EQ(0, sa.s[3]);
  stralloc_free(&sa);
}

/*
 * int stralloc_catulong0(stralloc* sa, unsigned long int in, size_t n);
 */

TEST(test_stralloc_catulong0) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_catulong0(&sa, 42, 5);
  ASSERT_EQ(1, is(&sa, "00042"));
  stralloc_zero(&sa);
  stralloc_catulong0(&sa, 12345, 2);
  ASSERT_EQ(1, is(&sa, "12345"));
  stralloc_free(&sa);
}

/*
 * int stralloc_catlong0(stralloc* sa, signed long int in, size_t n);
 */

TEST(test_stralloc_catlong0) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_catlong0(&sa, -42, 5);
  ASSERT_EQ(1, is(&sa, "-00042"));
  stralloc_zero(&sa);
  stralloc_catlong0(&sa, 42, 1);
  ASSERT_EQ(1, is(&sa, "42"));
  stralloc_free(&sa);
}

/*
 * void stralloc_free(stralloc* sa);
 */

TEST(test_stralloc_free) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "abc");
  stralloc_free(&sa);
  ASSERT_EQ(1, sa.s == NULL);
  ASSERT_EQ(0, sa.len);
  ASSERT_EQ(0, sa.a);
  /* reusable after free */ ASSERT_EQ(1, stralloc_copys(&sa, "again"));
  ASSERT_EQ(1, is(&sa, "again"));
  stralloc_free(&sa);
  stralloc_free(&sa);
  /* twice is fine */
}

/*
 * int stralloc_chop(stralloc* sa);
 */

TEST(test_stralloc_chop) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "abc");
  ASSERT_EQ('c', stralloc_chop(&sa));
  ASSERT_EQ(1, is(&sa, "ab"));
  stralloc_free(&sa);
}

/*
 * int stralloc_chomp(stralloc* sa);
 */

TEST(test_stralloc_chomp) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "line\n");
  ASSERT_EQ(1, stralloc_chomp(&sa));
  ASSERT_EQ(1, is(&sa, "line"));
  stralloc_copys(&sa, "line\r\n");
  ASSERT_EQ(2, stralloc_chomp(&sa));
  ASSERT_EQ(1, is(&sa, "line"));
  stralloc_copys(&sa, "x");
  ASSERT_EQ(0, stralloc_chomp(&sa));
  stralloc_free(&sa);
}

/*
 * void stralloc_trim(stralloc* sa, const char* trimchars, unsigned int trimcharslen);
 */

TEST(test_stralloc_trim) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "  x y  ");
  stralloc_trimr(&sa, " ", 1);
  ASSERT_EQ(1, is(&sa, "  x y"));
  stralloc_free(&sa);
}

/*
 * int stralloc_decamelize(stralloc *sa, stralloc *to);
 */

TEST(test_stralloc_decamelize) {
  stralloc sa;
  stralloc_init(&sa);
  ASSERT_EQ(11, stralloc_decamelize("fooBarBaz", &sa, '_'));
  ASSERT_EQ(1, is(&sa, "foo_bar_baz"));
  stralloc_free(&sa);
}

/*
 * int stralloc_case_diff(const stralloc *sa1, const stralloc *sa2);
 */

TEST(test_stralloc_case_diff) {
  stralloc a, b;
  stralloc_init(&a);
  stralloc_init(&b);
  stralloc_copys(&a, "Hello");
  stralloc_copys(&b, "hELLO");
  ASSERT_EQ(0, stralloc_case_diff(&a, &b));
  ASSERT_EQ(1, stralloc_case_equal(&a, &b));
  stralloc_copys(&b, "hELP");
  ASSERT_NE(0, stralloc_case_diff(&a, &b));
  stralloc_free(&a);
  stralloc_free(&b);
}

/*
 * int stralloc_case_diffs(const stralloc *sa, const char *s);
 */

TEST(test_stralloc_case_diffs) {
  stralloc a;
  stralloc_init(&a);
  stralloc_copys(&a, "Hello");
  ASSERT_EQ(0, stralloc_case_diffs(&a, "hello"));
  ASSERT_EQ(1, stralloc_case_equals(&a, "HELLO"));
  ASSERT_NE(0, stralloc_case_diffs(&a, "help"));
  stralloc_free(&a);
}

/*
 * int stralloc_diffb(const stralloc *sa, const void *d, unsigned int dlen);
 */

TEST(test_stralloc_diffb) {
  stralloc a;
  stralloc_init(&a);
  stralloc_copys(&a, "Hello");
  ASSERT_EQ(0, stralloc_diffb(&a, "Hello", 5));
  ASSERT_NE(0, stralloc_diffb(&a, "Hellp", 5));
  ASSERT_NE(0, stralloc_diffb(&a, "Hell", 4));
  ASSERT_EQ(1, stralloc_equalb(&a, "Hello", 5));
  stralloc_free(&a);
}

/*
 * size_t stralloc_scan(stralloc *out, const stralloc *in, size_t (*scan_function)(const char *, char *));
 */

TEST(test_stralloc_scan) {
  /* stralloc_scan needs a scan_function(const char*, char*) that converts in place: use a hex scanner */ stralloc in,
      out;
  stralloc_init(&in);
  stralloc_init(&out);
  stralloc_free(&in);
  stralloc_free(&out);
}

/*
 * int stralloc_insertb(stralloc* sa, const char* s, size_t pos, size_t n);
 */

TEST(test_stralloc_insertb) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "hello");
  ASSERT_EQ(1, stralloc_insertb(&sa, "XY", 2, 2));
  ASSERT_EQ(1, is(&sa, "heXYllo"));
  stralloc_free(&sa);
}

/*
 * int stralloc_inserts(stralloc* sa, const char* s, size_t pos);
 */

TEST(test_stralloc_inserts) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "hello");
  ASSERT_EQ(1, stralloc_inserts(&sa, "--", 0));
  ASSERT_EQ(1, is(&sa, "--hello"));
  stralloc_free(&sa);
}

/*
 * int stralloc_insert(stralloc* sa, const stralloc* other, size_t pos);
 */

TEST(test_stralloc_insert) {
  stralloc a, b;
  stralloc_init(&a);
  stralloc_init(&b);
  stralloc_copys(&a, "hello");
  stralloc_copys(&b, "++");
  ASSERT_EQ(1, stralloc_insert(&a, &b, 5));
  ASSERT_EQ(1, is(&a, "hello++"));
  stralloc_free(&a);
  stralloc_free(&b);
}

/*
 * int stralloc_subst(stralloc *out, const char *b, size_t len, const char *from, const char *to);
 */

TEST(test_stralloc_subst) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_subst(&sa, "a-b-c", 5, "-", "+");
  ASSERT_EQ(1, is(&sa, "a+b+c"));
  stralloc_free(&sa);
}

/*
 * size_t stralloc_fmt_call(stralloc*, stralloc_fmt_fn*, void* av[4]);
 */

TEST(test_stralloc_fmt_call) {
  /* stralloc_fmt_call(sa, fmt, av[4]): av[0]=input, av[1]=length, av[2]=fmt function */ stralloc sa;
  stralloc_init(&sa);
  stralloc_free(&sa);
}

/*
 * size_t stralloc_fmt_pred(stralloc*, const char* in, size_t in_len,  size_t (*fmt_function)(char*, unsigned int ch),
 * int (*pred)(int));
 */

TEST(test_stralloc_fmt_pred) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_fmt_pred(&sa, "aBcD", 4, fmt_angle, is_upper);
  ASSERT_EQ(1, is(&sa, "a<B>c<D>"));
  stralloc_free(&sa);
}

/*
 * size_t stralloc_fmt(stralloc*, const char* in, size_t in_len, size_t (*fmt_function)());
 */

TEST(test_stralloc_fmt) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_fmt(&sa, "ab", 2, fmt_angle);
  ASSERT_EQ(1, is(&sa, "<a><b>"));
  stralloc_free(&sa);
}

/*
 * int stralloc_catdouble(stralloc*, double d, int prec);
 */

TEST(test_stralloc_catdouble) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_catdouble(&sa, 3.14159, 2);
  ASSERT_EQ(1, is(&sa, "3.14"));
  stralloc_free(&sa);
}

/*
 * int  stralloc_copywcb(stralloc* sa, const wchar_t* buf, size_t len);
 */

TEST(test_stralloc_copywcb) {
  stralloc sa;
  stralloc_init(&sa);
  ASSERT_EQ(1, stralloc_copywcb(&sa, L"xyz", 2));
  ASSERT_EQ(1, is(&sa, "xy"));
  ASSERT_EQ(1, stralloc_copywcb(&sa, L"\x20ac", 1));
  ASSERT_EQ(1, is(&sa, "\xe2\x82\xac"));
  stralloc_free(&sa);
}

/*
 * int  stralloc_copywcs(stralloc* sa, const wchar_t* buf);
 */

TEST(test_stralloc_copywcs) {
  stralloc sa;
  stralloc_init(&sa);
  ASSERT_EQ(1, stralloc_copywcs(&sa, L"wide"));
  ASSERT_EQ(1, is(&sa, "wide"));
  stralloc_free(&sa);
}

/*
 * int stralloc_shrink(stralloc* sa);
 */

TEST(test_stralloc_shrink) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "abc");
  stralloc_ready(&sa, 1000);
  ASSERT_EQ(1, sa.a >= 1000);
  ASSERT_EQ(1, stralloc_shrink(&sa));
  ASSERT_EQ(3, sa.a);
  ASSERT_EQ(1, is(&sa, "abc"));
  stralloc_zero(&sa);
  ASSERT_EQ(1, stralloc_shrink(&sa));
  ASSERT_EQ(0, sa.a);
  stralloc_free(&sa);
}

/*
 * int stralloc_prependb(stralloc* sa, const char* s, size_t n);
 */

TEST(test_stralloc_prependb) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "world");
  ASSERT_EQ(1, stralloc_prependb(&sa, "ab", 1));
  ASSERT_EQ(1, is(&sa, "aworld"));
  stralloc_free(&sa);
}

/*
 * int stralloc_prepends(stralloc* sa, const char* s);
 */

TEST(test_stralloc_prepends) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "world");
  ASSERT_EQ(1, stralloc_prepends(&sa, "hello "));
  ASSERT_EQ(1, is(&sa, "hello world"));
  stralloc_free(&sa);
}

/*
 * int stralloc_prepend(stralloc* sa, const stralloc* other);
 */

TEST(test_stralloc_prepend) {
  stralloc a, b;
  stralloc_init(&a);
  stralloc_init(&b);
  stralloc_copys(&a, "world");
  stralloc_copys(&b, "++");
  ASSERT_EQ(1, stralloc_prepend(&a, &b));
  ASSERT_EQ(1, is(&a, "++world"));
  stralloc_free(&a);
  stralloc_free(&b);
}

/*
 * int stralloc_expand(stralloc* sa);
 */

TEST(test_stralloc_expand) {
  stralloc sa;
  stralloc_init(&sa);
  setenv("STRALLOC_EXPAND_TEST", "val", 1);
  stralloc_copys(&sa, "a $STRALLOC_EXPAND_TEST b ${STRALLOC_EXPAND_TEST}c $STRALLOC_NOT_SET_X");
  ASSERT_EQ(1, stralloc_expand(&sa));
  ASSERT_EQ(1, is(&sa, "a val b valc $STRALLOC_NOT_SET_X"));
  stralloc_free(&sa);
}

/*
 * int stralloc_catwcb(stralloc* sa, const wchar_t* buf, size_t len);
 */

TEST(test_stralloc_catwcb) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "wide");
  ASSERT_EQ(1, stralloc_catwcb(&sa, L"er!", 2));
  ASSERT_EQ(1, is(&sa, "wideer"));
  stralloc_free(&sa);
}

/*
 * int stralloc_catwcs(stralloc* sa, const wchar_t* buf);
 */

TEST(test_stralloc_catwcs) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "a");
  ASSERT_EQ(1, stralloc_catwcs(&sa, L"Q"));
  ASSERT_EQ(1, is(&sa, "aQ"));
  stralloc_free(&sa);
}

/*
 * int stralloc_remove(stralloc*, size_t pos, size_t n);
 */

TEST(test_stralloc_remove) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "abcdef");
  ASSERT_EQ(2, stralloc_remove(&sa, 1, 2));
  ASSERT_EQ(1, is(&sa, "adef"));
  stralloc_free(&sa);
}

/*
 * void stralloc_replacec(stralloc*, char before, char after);
 */

TEST(test_stralloc_replacec) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "adef");
  stralloc_replacec(&sa, 'e', 'E');
  ASSERT_EQ(1, is(&sa, "adEf"));
  stralloc_free(&sa);
}

/*
 * int stralloc_removesuffixs(stralloc*, const char* suffix);
 */

TEST(test_stralloc_removesuffixs) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "file.txt");
  ASSERT_EQ(1, stralloc_removesuffixs(&sa, ".txt"));
  ASSERT_EQ(1, is(&sa, "file"));
  stralloc_copys(&sa, "file.c");
  ASSERT_EQ(0, stralloc_removesuffixs(&sa, ".txt"));
  ASSERT_EQ(1, is(&sa, "file.c"));
  stralloc_free(&sa);
}

/*
 * int stralloc_removesuffixb(stralloc*, const char* x, size_t len);
 */

TEST(test_stralloc_removesuffixb) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "file.txt");
  ASSERT_EQ(1, stralloc_removesuffixb(&sa, ".txt", 4));
  ASSERT_EQ(1, is(&sa, "file"));
  stralloc_free(&sa);
}

/*
 * int stralloc_contains(const stralloc* sa, const char* what);
 */

TEST(test_stralloc_contains) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "hello");
  ASSERT_EQ(1, stralloc_contains(&sa, "ell"));
  ASSERT_EQ(0, stralloc_contains(&sa, "xyz"));
  stralloc_free(&sa);
}

/*
 * int stralloc_replaces(stralloc*, const char* from, const char* to);
 */

TEST(test_stralloc_replaces) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "aXbXc");
  ASSERT_EQ(1, stralloc_replaces(&sa, "X", "--"));
  ASSERT_EQ(1, is(&sa, "a--b--c"));
  stralloc_free(&sa);
}

/*
 * int stralloc_replace(stralloc*, size_t pos, size_t len, const char* to, size_t tolen);
 */

TEST(test_stralloc_replace) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, "abcdef");
  ASSERT_EQ(1, stralloc_replace(&sa, 1, 3, "ZZ", 2));
  ASSERT_EQ(1, is(&sa, "aZZef"));
  stralloc_free(&sa);
}

/*
 * the rest of the stralloc API that had no test
 */
TEST(test_stralloc_numbers) {
  stralloc sa;

  stralloc_init(&sa);
  stralloc_catulong(&sa, 12345);
  stralloc_catuint(&sa, 7);
  stralloc_catxlong(&sa, 255);
  ASSERT_EQ(1, is(&sa, "123457ff"));
  stralloc_zero(&sa);
  stralloc_catxlong0(&sa, 255, 6);
  ASSERT_EQ(1, is(&sa, "0000ff"));
  stralloc_zero(&sa);
  stralloc_catint(&sa, -5);
  stralloc_catint0(&sa, 3, 3);
  stralloc_catuint0(&sa, 9, 2);
  ASSERT_EQ(1, is(&sa, "-500309"));
  stralloc_free(&sa);
}

TEST(test_stralloc_append) {
  stralloc sa;

  stralloc_init(&sa);
  ASSERT_EQ(1, stralloc_append(&sa, "xyz")); /* takes a pointer to ONE char */
  ASSERT_EQ(1, is(&sa, "x"));
  stralloc_0(&sa); /* appends a NUL byte, counted */
  ASSERT_EQ(2, sa.len);
  ASSERT_EQ(0, sa.s[1]);
  stralloc_zero(&sa);
  stralloc_APPEND(&sa, "q");
  ASSERT_EQ(1, is(&sa, "q"));
  stralloc_free(&sa);
}

TEST(test_stralloc_search) {
  stralloc a, b;

  stralloc_init(&a);
  stralloc_init(&b);
  stralloc_copys(&a, "Hello");
  stralloc_copys(&b, "llo");
  ASSERT_EQ(2, stralloc_find(&a, &b));
  ASSERT_EQ(2, stralloc_findb(&a, "l", 1));
  ASSERT_EQ(3, stralloc_finds(&a, "lo"));
  ASSERT_EQ(5, stralloc_finds(&a, "zz")); /* not found: the length, like str_chr */
  ASSERT_EQ(2, stralloc_chr(&a, 'l'));
  ASSERT_EQ(3, stralloc_rchr(&a, 'l'));
  ASSERT_EQ(2, stralloc_count(&a, 'l'));
  ASSERT_EQ(1, stralloc_starts(&a, "Hel"));
  ASSERT_EQ(1, stralloc_startb(&a, "Hel", 3));
  ASSERT_EQ(1, stralloc_start(&a, &a));
  ASSERT_EQ(1, stralloc_case_starts(&a, "hEL"));
  ASSERT_EQ(1, stralloc_endsa(&a, &b));
  ASSERT_EQ(1, stralloc_endb(&a, "llo", 3));
  ASSERT_EQ(1, stralloc_case_end(&a, &b));
  stralloc_copys(&b, "hel");
  ASSERT_EQ(1, stralloc_case_start(&a, &b));
  stralloc_free(&a);
  stralloc_free(&b);
}

TEST(test_stralloc_transform) {
  stralloc sa;

  stralloc_init(&sa);
  stralloc_copys(&sa, "abc");
  stralloc_upper(&sa);
  ASSERT_EQ(1, is(&sa, "ABC"));
  stralloc_lower(&sa);
  ASSERT_EQ(1, is(&sa, "abc"));
  stralloc_reverse(&sa);
  ASSERT_EQ(1, is(&sa, "cba"));
  stralloc_copys(&sa, "abcdef");
  stralloc_reverse_blocks(&sa, 2);
  ASSERT_EQ(1, is(&sa, "cdabef"));
  stralloc_copys(&sa,
                 "a\x01"
                 "b\x7f");
  stralloc_replace_non_printable(&sa, '.');
  ASSERT_EQ(1, is(&sa, "a.b."));
  stralloc_copys(&sa, "a-b_c");
  stralloc_remove_all(&sa, "-_", 2);
  ASSERT_EQ(1, is(&sa, "abc"));
  stralloc_zero(&sa);
  stralloc_cathexb(&sa, "\x01\xab", 2);
  ASSERT_EQ(1, is(&sa, "01ab"));
  stralloc_free(&sa);
}

TEST(test_stralloc_move_erase) {
  stralloc a, c;
  size_t len = 0;
  char* p;

  stralloc_init(&a);
  stralloc_init(&c);
  stralloc_copys(&a, "moved");
  stralloc_move(&c, &a);
  ASSERT_EQ(1, is(&c, "moved"));
  ASSERT_EQ(1, a.s == NULL); /* the source gave the memory away */
  stralloc_free(&c);

  stralloc_copys(&a, "abc");
  p = stralloc_moveb(&a, &len);
  ASSERT_EQ(3, len);
  ASSERT_EQ(0, byte_diff(p, 3, "abc"));
  ASSERT_EQ(1, a.s == NULL);
  free(p);

  stralloc_copys(&a, "x");
  ASSERT_EQ(1, stralloc_erase(&a));
  ASSERT_EQ(0, a.len);
  stralloc_free(&a);
}

TEST(test_stralloc_catq) {
  stralloc sa;

  stralloc_init(&sa);
  stralloc_catq(&sa, "foo bar", 7, "'");
  ASSERT_EQ(1, is(&sa, "'foo' 'bar'")); /* every blank separated word is quoted */
  stralloc_zero(&sa);
  stralloc_catq(&sa, "foo", 3, ""); /* no quote: copied as is */
  ASSERT_EQ(1, is(&sa, "foo"));
  stralloc_free(&sa);
}

TEST(test_stralloc_ready_tuned) {
  stralloc sa;

  stralloc_init(&sa);
  ASSERT_EQ(1, stralloc_ready(&sa, 10));
  ASSERT_EQ(1, sa.a >= 10);
  ASSERT_EQ(1, stralloc_readyplus(&sa, 100));
  ASSERT_EQ(1, sa.a >= 100);
  ASSERT_EQ(1, stralloc_ready_tuned(&sa, 5000, 16, 1, 2));
  ASSERT_EQ(1, sa.a >= 5000);
  ASSERT_EQ(0, stralloc_ready_tuned(&sa, 10, 0, 1, 0)); /* b == 0 is invalid */
  stralloc_free(&sa);
}

TEST(test_stralloc_scan_stops) {
  /* a scan function that consumes nothing must not make stralloc_scan loop forever */
  stralloc in, out;

  stralloc_init(&in);
  stralloc_init(&out);
  stralloc_copys(&in, "ab");
  stralloc_scan(&out, &in, (size_t (*)(const char*, char*))scan_nothing);
  ASSERT_EQ(0, out.len);
  stralloc_free(&in);
  stralloc_free(&out);
}

/*
 * stralloc_diff, stralloc_equal, stralloc_equals, stralloc_alloc
 */
TEST(test_stralloc_diff_equal_alloc) {
  stralloc a, b, c;

  stralloc_init(&a);
  stralloc_init(&b);
  stralloc_init(&c);
  stralloc_copys(&a, "abc");
  stralloc_copys(&b, "abd");
  stralloc_copys(&c, "abc");

  ASSERT_EQ(1, stralloc_diff(&a, &b) < 0);
  ASSERT_EQ(1, stralloc_diff(&b, &a) > 0);
  ASSERT_EQ(0, stralloc_diff(&a, &c));
  ASSERT_EQ(1, stralloc_equal(&a, &c));
  ASSERT_EQ(0, stralloc_equal(&a, &b));
  ASSERT_EQ(1, stralloc_equals(&a, "abc"));
  ASSERT_EQ(0, stralloc_equals(&a, "abcd"));
  stralloc_free(&a);
  stralloc_free(&b);
  stralloc_free(&c);

  /* stralloc_alloc turns a stralloc that only points to a string into an owned copy */
  {
    stralloc v;
    static char lit[] = "borrowed";

    byte_zero(&v, sizeof v);
    v.s = lit;
    stralloc_alloc(&v);
    ASSERT_EQ(8, v.len);
    ASSERT_EQ(1, v.s != lit);
    ASSERT_EQ(0, byte_diff(v.s, 8, "borrowed"));
    ASSERT_EQ(1, v.a >= 9);
    stralloc_free(&v);
  }
}

/*
 * int buffer_getln(buffer*, stralloc*, int* gotit, int termchar);
 */
TEST(test_buffer_getln) {
  stralloc src, line;
  buffer b;
  int got = -1;

  stralloc_init(&src);
  stralloc_init(&line);
  stralloc_copys(&src, "one\ntwo\nlast");
  buffer_fromsa(&b, &src);

  ASSERT_EQ(0, buffer_getln(&b, &line, &got, '\n'));
  ASSERT_EQ(1, got);
  ASSERT_EQ(4, line.len);
  ASSERT_EQ(0, byte_diff(line.s, 4, "one\n"));
  ASSERT_EQ(0, buffer_getln(&b, &line, &got, '\n'));
  ASSERT_EQ(1, got);
  ASSERT_EQ(0, byte_diff(line.s, 4, "two\n"));
  /* the last line has no terminator: gotit is 0 and the text is there */
  ASSERT_EQ(0, buffer_getln(&b, &line, &got, '\n'));
  ASSERT_EQ(0, got);
  ASSERT_EQ(4, line.len);
  ASSERT_EQ(0, byte_diff(line.s, 4, "last"));
  /* end of file: nothing left */
  ASSERT_EQ(0, buffer_getln(&b, &line, &got, '\n'));
  ASSERT_EQ(0, got);
  ASSERT_EQ(0, line.len);

  stralloc_free(&line);
  stralloc_free(&src);
}

/*
 * ssize_t readclose(fd, sa, bufsize), readclose_append, int openreadclose(fn, sa, bufsize)
 */
TEST(test_readclose_openreadclose) {
  const char* fn = "/tmp/c-utils-readclose.tmp";
  stralloc sa;
  int fd;

  stralloc_init(&sa);
  fd = open_trunc(fn);
  ASSERT_EQ(1, fd >= 0);
  write(fd, "hello, world", 12);
  close(fd);

  ASSERT_EQ(1, openreadclose(fn, &sa, 4));
  ASSERT_EQ(12, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s, 12, "hello, world"));

  fd = open_read(fn);
  ASSERT_EQ(0, readclose(fd, &sa, 1)); /* 0 at end of file; replaces */
  ASSERT_EQ(12, sa.len);

  fd = open_read(fn);
  ASSERT_EQ(0, readclose_append(fd, &sa, 100)); /* appends */
  ASSERT_EQ(24, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s + 12, 12, "hello, world"));

  ASSERT_EQ(0, openreadclose("/nonexistent-dir/none", &sa, 8)); /* missing file: 0 */
  stralloc_free(&sa);
  unlink(fn);
}

#define RUN_STRALLOC_TESTS() \
  RUN(test_stralloc_numbers); \
  RUN(test_stralloc_append); \
  RUN(test_stralloc_search); \
  RUN(test_stralloc_transform); \
  RUN(test_stralloc_move_erase); \
  RUN(test_stralloc_catq); \
  RUN(test_stralloc_ready_tuned); \
  RUN(test_stralloc_scan_stops); \
  RUN(test_stralloc_init); \
  RUN(test_stralloc_ready); \
  RUN(test_stralloc_readyplus); \
  RUN(test_stralloc_copyb); \
  RUN(test_stralloc_copys); \
  RUN(test_stralloc_copy); \
  RUN(test_stralloc_catb); \
  RUN(test_stralloc_cats); \
  RUN(test_stralloc_catc); \
  RUN(test_stralloc_zero); \
  RUN(test_stralloc_trunc); \
  RUN(test_stralloc_catm_internal); \
  RUN(test_stralloc_cat); \
  RUN(test_stralloc_diffs); \
  RUN(test_stralloc_nul); \
  RUN(test_stralloc_catulong0); \
  RUN(test_stralloc_catlong0); \
  RUN(test_stralloc_free); \
  RUN(test_stralloc_chop); \
  RUN(test_stralloc_chomp); \
  RUN(test_stralloc_trim); \
  RUN(test_stralloc_decamelize); \
  RUN(test_stralloc_case_diff); \
  RUN(test_stralloc_case_diffs); \
  RUN(test_stralloc_diffb); \
  RUN(test_stralloc_diffs); \
  RUN(test_stralloc_scan); \
  RUN(test_stralloc_insertb); \
  RUN(test_stralloc_inserts); \
  RUN(test_stralloc_insert); \
  RUN(test_stralloc_subst); \
  RUN(test_stralloc_fmt_call); \
  RUN(test_stralloc_fmt_pred); \
  RUN(test_stralloc_fmt); \
  RUN(test_stralloc_catdouble); \
  RUN(test_stralloc_copywcb); \
  RUN(test_stralloc_copywcs); \
  RUN(test_stralloc_shrink); \
  RUN(test_stralloc_prependb); \
  RUN(test_stralloc_prepends); \
  RUN(test_stralloc_prepend); \
  RUN(test_stralloc_expand); \
  RUN(test_stralloc_catwcb); \
  RUN(test_stralloc_catwcs); \
  RUN(test_stralloc_remove); \
  RUN(test_stralloc_replacec); \
  RUN(test_stralloc_removesuffixs); \
  RUN(test_stralloc_removesuffixb); \
  RUN(test_stralloc_contains); \
  RUN(test_stralloc_replaces); \
  RUN(test_stralloc_replace); \
  RUN(test_stralloc_diff_equal_alloc); \
  RUN(test_buffer_getln); \
  RUN(test_readclose_openreadclose);

TESTS(stralloc) { RUN_STRALLOC_TESTS(); }
