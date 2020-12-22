#include "unit_test.h"
#include "../lib/stralloc.h"

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

TEST(test_stralloc_copyb) {}

/*
 * int stralloc_copys(stralloc* sa, const char* buf);
 */

TEST(test_stralloc_copys) {}

/*
 * int stralloc_copy(stralloc* sa, const stralloc* sa2);
 */

TEST(test_stralloc_copy) {}

/*
 * int stralloc_catb(stralloc* sa, const char* in, size_t len);
 */

TEST(test_stralloc_catb) {}

/*
 * int stralloc_cats(stralloc* sa, const char* in);
 */

TEST(test_stralloc_cats) {}

/*
 * int stralloc_catc(stralloc* sa, const unsigned char c);
 */

TEST(test_stralloc_catc) {}

/*
 * void stralloc_zero(stralloc* sa);
 */

TEST(test_stralloc_zero) {}

/*
 * int stralloc_trunc(stralloc* sa, size_t n);
 */

TEST(test_stralloc_trunc) {}

/*
 * int stralloc_catm_internal(stralloc* sa, ...);
 */

TEST(test_stralloc_catm_internal) {}

/*
 * int stralloc_cat(stralloc* sa, const stralloc* in);
 */

TEST(test_stralloc_cat) {}

/*
 * int stralloc_diffs(const stralloc *a, const char *b);
 */

TEST(test_stralloc_diffs) {}

/*
 * int stralloc_nul(stralloc* sa);
 */

TEST(test_stralloc_nul) {}

/*
 * int stralloc_catulong0(stralloc* sa, unsigned long int in, size_t n);
 */

TEST(test_stralloc_catulong0) {}

/*
 * int stralloc_catlong0(stralloc* sa, signed long int in, size_t n);
 */

TEST(test_stralloc_catlong0) {}

/*
 * void stralloc_free(stralloc* sa);
 */

TEST(test_stralloc_free) {}

/*
 * int stralloc_chop(stralloc* sa);
 */

TEST(test_stralloc_chop) {}

/*
 * int stralloc_chomp(stralloc* sa);
 */

TEST(test_stralloc_chomp) {}

/*
 * void stralloc_trim(stralloc* sa, const char* trimchars, unsigned int trimcharslen);
 */

TEST(test_stralloc_trim) {}

/*
 * int stralloc_decamelize(stralloc *sa, stralloc *to);
 */

TEST(test_stralloc_decamelize) {}

/*
 * int stralloc_case_diff(const stralloc *sa1, const stralloc *sa2);
 */

TEST(test_stralloc_case_diff) {}

/*
 * int stralloc_case_diffs(const stralloc *sa, const char *s);
 */

TEST(test_stralloc_case_diffs) {}

/*
 * int stralloc_diffb(const stralloc *sa, const void *d, unsigned int dlen);
 */

TEST(test_stralloc_diffb) {}

/*
 * size_t stralloc_scan(stralloc *out, const stralloc *in, size_t (*scan_function)(const char *, char *));
 */

TEST(test_stralloc_scan) {}

/*
 * int stralloc_insertb(stralloc* sa, const char* s, size_t pos, size_t n);
 */

TEST(test_stralloc_insertb) {}

/*
 * int stralloc_inserts(stralloc* sa, const char* s, size_t pos);
 */

TEST(test_stralloc_inserts) {}

/*
 * int stralloc_insert(stralloc* sa, const stralloc* other, size_t pos);
 */

TEST(test_stralloc_insert) {}

/*
 * int stralloc_subst(stralloc *out, const char *b, size_t len, const char *from, const char *to);
 */

TEST(test_stralloc_subst) {}

/*
 * size_t stralloc_fmt_call(stralloc*, stralloc_fmt_fn*, void* av[4]);
 */

TEST(test_stralloc_fmt_call) {}

/*
 * size_t stralloc_fmt_pred(stralloc*, const char* in, size_t in_len,  size_t (*fmt_function)(char*, unsigned int ch),
 * int (*pred)(int));
 */

TEST(test_stralloc_fmt_pred) {}

/*
 * size_t stralloc_fmt(stralloc*, const char* in, size_t in_len, size_t (*fmt_function)());
 */

TEST(test_stralloc_fmt) {}

/*
 * int stralloc_catdouble(stralloc*, double d, int prec);
 */

TEST(test_stralloc_catdouble) {}

/*
 * int  stralloc_copywcb(stralloc* sa, const wchar_t* buf, size_t len);
 */

TEST(test_stralloc_copywcb) {}

/*
 * int  stralloc_copywcs(stralloc* sa, const wchar_t* buf);
 */

TEST(test_stralloc_copywcs) {}

/*
 * int stralloc_shrink(stralloc* sa);
 */

TEST(test_stralloc_shrink) {}

/*
 * int stralloc_prependb(stralloc* sa, const char* s, size_t n);
 */

TEST(test_stralloc_prependb) {}

/*
 * int stralloc_prepends(stralloc* sa, const char* s);
 */

TEST(test_stralloc_prepends) {}

/*
 * int stralloc_prepend(stralloc* sa, const stralloc* other);
 */

TEST(test_stralloc_prepend) {}

/*
 * int stralloc_expand(stralloc* sa);
 */

TEST(test_stralloc_expand) {}

/*
 * int stralloc_catwcb(stralloc* sa, const wchar_t* buf, size_t len);
 */

TEST(test_stralloc_catwcb) {}

/*
 * int stralloc_catwcs(stralloc* sa, const wchar_t* buf);
 */

TEST(test_stralloc_catwcs) {}

/*
 * int stralloc_remove(stralloc*, size_t pos, size_t n);
 */

TEST(test_stralloc_remove) {}

/*
 * void stralloc_replacec(stralloc*, char before, char after);
 */

TEST(test_stralloc_replacec) {}

/*
 * int stralloc_removesuffixs(stralloc*, const char* suffix);
 */

TEST(test_stralloc_removesuffixs) {}

/*
 * int stralloc_removesuffixb(stralloc*, const char* x, size_t len);
 */

TEST(test_stralloc_removesuffixb) {}

/*
 * int stralloc_contains(const stralloc* sa, const char* what);
 */

TEST(test_stralloc_contains) {}

/*
 * int stralloc_replaces(stralloc*, const char* from, const char* to);
 */

TEST(test_stralloc_replaces) {}

/*
 * int stralloc_replace(stralloc*, size_t pos, size_t len, const char* to, size_t tolen);
 */

TEST(test_stralloc_replace) {}

#define RUN_STRALLOC_TESTS()                                                                                                                                                                           \
  RUN(test_stralloc_init);                                                                                                                                                                             \
  RUN(test_stralloc_ready);                                                                                                                                                                            \
  RUN(test_stralloc_readyplus);                                                                                                                                                                        \
  RUN(test_stralloc_copyb);                                                                                                                                                                            \
  RUN(test_stralloc_copys);                                                                                                                                                                            \
  RUN(test_stralloc_copy);                                                                                                                                                                             \
  RUN(test_stralloc_catb);                                                                                                                                                                             \
  RUN(test_stralloc_cats);                                                                                                                                                                             \
  RUN(test_stralloc_catc);                                                                                                                                                                             \
  RUN(test_stralloc_zero);                                                                                                                                                                             \
  RUN(test_stralloc_trunc);                                                                                                                                                                            \
  RUN(test_stralloc_catm_internal);                                                                                                                                                                    \
  RUN(test_stralloc_cat);                                                                                                                                                                              \
  RUN(test_stralloc_diffs);                                                                                                                                                                            \
  RUN(test_stralloc_nul);                                                                                                                                                                              \
  RUN(test_stralloc_catulong0);                                                                                                                                                                        \
  RUN(test_stralloc_catlong0);                                                                                                                                                                         \
  RUN(test_stralloc_free);                                                                                                                                                                             \
  RUN(test_stralloc_chop);                                                                                                                                                                             \
  RUN(test_stralloc_chomp);                                                                                                                                                                            \
  RUN(test_stralloc_trim);                                                                                                                                                                             \
  RUN(test_stralloc_decamelize);                                                                                                                                                                       \
  RUN(test_stralloc_case_diff);                                                                                                                                                                        \
  RUN(test_stralloc_case_diffs);                                                                                                                                                                       \
  RUN(test_stralloc_diffb);                                                                                                                                                                            \
  RUN(test_stralloc_diffs);                                                                                                                                                                            \
  RUN(test_stralloc_scan);                                                                                                                                                                             \
  RUN(test_stralloc_insertb);                                                                                                                                                                          \
  RUN(test_stralloc_inserts);                                                                                                                                                                          \
  RUN(test_stralloc_insert);                                                                                                                                                                           \
  RUN(test_stralloc_subst);                                                                                                                                                                            \
  RUN(test_stralloc_fmt_call);                                                                                                                                                                         \
  RUN(test_stralloc_fmt_pred);                                                                                                                                                                         \
  RUN(test_stralloc_fmt);                                                                                                                                                                              \
  RUN(test_stralloc_catdouble);                                                                                                                                                                        \
  RUN(test_stralloc_copywcb);                                                                                                                                                                          \
  RUN(test_stralloc_copywcs);                                                                                                                                                                          \
  RUN(test_stralloc_shrink);                                                                                                                                                                           \
  RUN(test_stralloc_prependb);                                                                                                                                                                         \
  RUN(test_stralloc_prepends);                                                                                                                                                                         \
  RUN(test_stralloc_prepend);                                                                                                                                                                          \
  RUN(test_stralloc_expand);                                                                                                                                                                           \
  RUN(test_stralloc_catwcb);                                                                                                                                                                           \
  RUN(test_stralloc_catwcs);                                                                                                                                                                           \
  RUN(test_stralloc_remove);                                                                                                                                                                           \
  RUN(test_stralloc_replacec);                                                                                                                                                                         \
  RUN(test_stralloc_removesuffixs);                                                                                                                                                                    \
  RUN(test_stralloc_removesuffixb);                                                                                                                                                                    \
  RUN(test_stralloc_contains);                                                                                                                                                                         \
  RUN(test_stralloc_replaces);                                                                                                                                                                         \
  RUN(test_stralloc_replace);

TESTS(stralloc) { RUN_STRALLOC_TESTS(); }
