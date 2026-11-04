#include "unit_test.h"
#include "../lib/buffer.h"
#include "../lib/strlist.h"
#include "../lib/stralloc.h"
#include "../lib/byte.h"
#include "../lib/str.h"
#include <stdlib.h>

/*
 * size_t strlist_count(const strlist* sl);
 */
TEST(test_strlist_count) {
  size_t n;
  strlist sl;
  strlist_init(&sl, '\0');
  strlist_push(&sl, "1");
  strlist_push(&sl, "2");
  strlist_push(&sl, "3");
  strlist_push(&sl, "4");

  n = strlist_count(&sl);
  ASSERT_EQ(4, n);
}

/*
 * char* strlist_at(const strlist* sl, size_t i);
 */
TEST(test_strlist_at) {
  strlist sl;
  strlist_init(&sl, '\0');
  strlist_push(&sl, "1");
  strlist_push(&sl, "2");
  strlist_push(&sl, "3");
  strlist_push(&sl, "4");

  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 0), "1"));
  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 1), "2"));
  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 2), "3"));
  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 3), "4"));
}

/*
 * char* strlist_at_n(const strlist* sl, size_t i, size_t* n);
 */
TEST(test_strlist_at_n) {
  const char* s;
  size_t n;
  strlist sl;
  strlist_init(&sl, '\0');
  strlist_push(&sl, "1");
  strlist_push(&sl, "12");
  strlist_push(&sl, "123");
  strlist_push(&sl, "1234");

  s = strlist_at_n(&sl, 0, &n);
  ASSERT_EQ(n, 1);
  ASSERT_EQ(0, !str_equal(s, "1"));

  s = strlist_at_n(&sl, 1, &n);
  ASSERT_EQ(n, 2);
  ASSERT_EQ(0, !str_equal(s, "12"));

  s = strlist_at_n(&sl, 2, &n);
  ASSERT_EQ(n, 3);
  ASSERT_EQ(0, !str_equal(s, "123"));

  s = strlist_at_n(&sl, 3, &n);
  ASSERT_EQ(n, 4);
  ASSERT_EQ(0, !str_equal(s, "1234"));
}

/*
 * stralloc strlist_at_sa(const strlist* sl, size_t i);
 */
TEST(test_strlist_at_sa) {
  stralloc sa;
  strlist sl;
  strlist_init(&sl, '\0');
  strlist_push(&sl, "1");
  strlist_push(&sl, "12");
  strlist_push(&sl, "123");
  strlist_push(&sl, "1234");
  stralloc_init(&sa);

  strlist_at_sa(&sl, &sa, 0);
  ASSERT_EQ(0, !stralloc_equals(&sa, "1"));
  strlist_at_sa(&sl, &sa, 1);
  ASSERT_EQ(0, !stralloc_equals(&sa, "12"));
  strlist_at_sa(&sl, &sa, 2);
  ASSERT_EQ(0, !stralloc_equals(&sa, "123"));
  strlist_at_sa(&sl, &sa, 3);
  ASSERT_EQ(0, !stralloc_equals(&sa, "1234"));
}

/*
 * int strlist_cat(strlist* sl, const strlist* l);
 */
TEST(test_strlist_cat) {
  stralloc sa;
  strlist sl1, sl2;
  stralloc_init(&sa);

  strlist_init(&sl1, ',');
  strlist_init(&sl2, '\0');

  strlist_push(&sl1, "blah");
  strlist_push(&sl1, "l33t");

  strlist_push(&sl2, "1234");
  strlist_push(&sl2, "8192");

  strlist_cat(&sl1, &sl2);

  buffer_putsa(buffer_1, &sl1.sa);
  buffer_putnlflush(buffer_1);

  ASSERT_EQ(0, str_diffn(strlist_at(&sl1, 2), "1234", 4));
  strlist_join(&sl1, &sa, '|');

  buffer_putsa(buffer_1, &sa);
  buffer_putnlflush(buffer_1);

  ASSERT_EQ(0, !stralloc_equals(&sa, "blah|l33t|1234|8192"));

  ASSERT_EQ(4, strlist_count(&sl1));
}

/*
 * int strlist_contains(strlist* sl, const char* s);
 */
TEST(test_strlist_contains) {
  strlist sl;
  strlist_init(&sl, '\0');
  strlist_push(&sl, "1");
  strlist_push(&sl, "12");
  strlist_push(&sl, "123");
  strlist_push(&sl, "1234");

  ASSERT_EQ(0, !strlist_contains(&sl, "12"));
  ASSERT_EQ(0, strlist_contains(&sl, "4321"));
  ASSERT_EQ(0, strlist_contains(&sl, "43210"));
}

/*
 * int strlist_contains_sa(strlist* sl, const stralloc* sa);
 */
TEST(test_strlist_contains_sa) {
  stralloc sa;
  strlist sl;
  stralloc_init(&sa);
  strlist_init(&sl, '\0');
  strlist_push(&sl, "1");
  strlist_push(&sl, "12");
  strlist_push(&sl, "123");
  strlist_push(&sl, "1234");

  stralloc_copys(&sa, "12");
  ASSERT_EQ(0, !strlist_contains_sa(&sl, &sa));
  stralloc_copys(&sa, "4321");
  ASSERT_EQ(0, strlist_contains_sa(&sl, &sa));
  stralloc_copys(&sa, "12340");
  ASSERT_EQ(0, strlist_contains_sa(&sl, &sa));
}

/*
 * void strlist_dump(buffer* out, const strlist* sl);
 */
TEST(test_strlist_dump) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * void strlist_froms(strlist* sl, const char* s, char delim);
 */
TEST(test_strlist_froms) {
  strlist sl;
  strlist_init(&sl, '\0');
  strlist_froms(&sl, "blah|l33t|1234|8192", '|');

  ASSERT_EQ(4, strlist_count(&sl));

  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 0), "blah"));
  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 1), "l33t"));
  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 2), "1234"));
  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 3), "8192"));
}

/*
 * void    strlist_fromb(strlist*, const char* x, size_t n, const char* delim);
 */

TEST(test_strlist_fromb) {
  strlist sl;
  const char* x = "blah / l33t / 1234 / 8192";
  strlist_init(&sl, '\0');
  strlist_fromb(&sl, x, str_len(x), " / ");

  ASSERT_EQ(4, strlist_count(&sl));

  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 0), "blah"));
  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 1), "l33t"));
  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 2), "1234"));
  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 3), "8192"));
}

/*
 * int64 strlist_index_of(strlist* sl, const char* str);
 */
TEST(test_strlist_index_of) {
  strlist sl;
  strlist_init(&sl, '\0');
  strlist_froms(&sl, "blah|l33t|1234|8192", '|');

  ASSERT_EQ(15, strlist_index_of(&sl, "8192"));
  ASSERT_EQ(10, strlist_index_of(&sl, "1234"));
  ASSERT_EQ(5, strlist_index_of(&sl, "l33t"));
  ASSERT_EQ(0, strlist_index_of(&sl, "blah"));
}

/*
 * void strlist_join(const strlist* sl, stralloc* sa, char delim);
 */
TEST(test_strlist_join) {
  stralloc sa;
  strlist sl;
  stralloc_init(&sa);

  strlist_init(&sl, '\0');
  strlist_push(&sl, "water");
  strlist_push(&sl, "soil");
  strlist_push(&sl, "air");
  strlist_push(&sl, "fire");

  strlist_join(&sl, &sa, ',');

  ASSERT_EQ(0, !stralloc_equals(&sa, "water,soil,air,fire"));
}

/*
 * void strlist_joins(const strlist* sl, stralloc* sa, const char* delim);
 */
TEST(test_strlist_joins) {
  stralloc sa;
  strlist sl;
  stralloc_init(&sa);

  strlist_init(&sl, '\0');
  strlist_push(&sl, "water");
  strlist_push(&sl, "soil");
  strlist_push(&sl, "air");
  strlist_push(&sl, "fire");

  strlist_joins(&sl, &sa, "; ");

  ASSERT_EQ(0, !stralloc_equals(&sa, "water; soil; air; fire"));
}

/*
 * int strlist_push(strlist* sl, const char* s);
 */
TEST(test_strlist_push) {
  strlist sl;
  strlist_init(&sl, '\0');

  ASSERT_EQ(0, strlist_count(&sl));
  strlist_push(&sl, "water");
  ASSERT_EQ(1, strlist_count(&sl));
  strlist_push(&sl, "soil");
  ASSERT_EQ(2, strlist_count(&sl));
  strlist_push(&sl, "air");
  ASSERT_EQ(3, strlist_count(&sl));
  strlist_push(&sl, "fire");
  ASSERT_EQ(4, strlist_count(&sl));
}

/*
 * int strlist_push_sa(strlist* sl, const stralloc* sa);
 */
TEST(test_strlist_push_sa) {
  stralloc sa;
  strlist sl;
  stralloc_init(&sa);

  strlist_init(&sl, '|');

  stralloc_copys(&sa, "stralloc");
  strlist_push_sa(&sl, &sa);
  strlist_push_sa(&sl, &sa);

  ASSERT_EQ(0, !stralloc_equals(&sl.sa, "stralloc|stralloc"));
}

/*
 * void strlist_push_tokens(strlist* sl, const char* s, const char* delim);
 */
TEST(test_strlist_push_tokens) {
  const char* s;
  strlist sl;
  strlist_init(&sl, '\0');

  strlist_push_tokens(&sl, "blah, l33t, 1234, 8192", ", ");

  s = strlist_at(&sl, 3);
  ASSERT_EQ(0, !str_equal(s, "8192"));

  s = strlist_at(&sl, 2);
  ASSERT_EQ(0, !str_equal(s, "1234"));
}

/*
 * int strlist_push_unique(strlist* sl, const char* s);
 */
TEST(test_strlist_push_unique) {
  strlist sl;
  strlist_init(&sl, '\0');

  ASSERT_EQ(0, strlist_count(&sl));
  strlist_push_unique(&sl, "strlist");
  ASSERT_EQ(1, strlist_count(&sl));
  strlist_push_unique(&sl, "stralloc");
  ASSERT_EQ(2, strlist_count(&sl));
  strlist_push_unique(&sl, "stralloc");
  ASSERT_EQ(2, strlist_count(&sl));
  strlist_push_unique(&sl, "str");
  ASSERT_EQ(3, strlist_count(&sl));
  strlist_push_unique(&sl, "stralloc");
  ASSERT_EQ(3, strlist_count(&sl));
}

/*
 * int strlist_push_unique_sa(strlist* sl, const stralloc* sa);
 */
TEST(test_strlist_push_unique_sa) {
  stralloc sa;
  strlist sl;
  stralloc_init(&sa);

  strlist_init(&sl, '|');

  ASSERT_EQ(0, strlist_count(&sl));
  stralloc_copys(&sa, "stralloc");
  strlist_push_unique_sa(&sl, &sa);
  ASSERT_EQ(1, strlist_count(&sl));
  strlist_push_unique_sa(&sl, &sa);
  ASSERT_EQ(1, strlist_count(&sl));
  stralloc_copys(&sa, "strlist");
  strlist_push_unique_sa(&sl, &sa);
  ASSERT_EQ(2, strlist_count(&sl));
  stralloc_copys(&sa, "str");
  strlist_push_unique_sa(&sl, &sa);
  ASSERT_EQ(3, strlist_count(&sl));
  stralloc_copys(&sa, "stralloc");
  strlist_push_unique_sa(&sl, &sa);
  ASSERT_EQ(3, strlist_count(&sl));
}

/*
 * int strlist_pushb(strlist* sl, const char* s, size_t n);
 */
TEST(test_strlist_pushb) {
  stralloc sa;
  strlist sl;
  stralloc_init(&sa);
  strlist_init(&sl, '\0');

  strlist_pushb(&sl, "bebe", 4);
  strlist_pushb(&sl, "bebe", 3);
  strlist_pushb(&sl, "bebe", 2);
  strlist_pushb(&sl, "bebe", 1);

  strlist_joins(&sl, &sa, "; ");
  buffer_putsa(buffer_1, &sa);
  buffer_putnlflush(buffer_1);

  ASSERT_EQ(0, !stralloc_equals(&sa, "bebe; beb; be; b"));
}

/*
 * int strlist_pushm_internal(strlist* sl, ...);
 */
TEST(test_strlist_pushm) {
  stralloc sa;
  strlist sl;
  stralloc_init(&sa);
  strlist_init(&sl, '\0');

  strlist_pushm_internal(&sl, "abcd", "abc", "ab", "a", 0);

  strlist_joins(&sl, &sa, " | ");

  ASSERT_EQ(0, !stralloc_equals(&sa, "abcd | abc | ab | a"));
}

/*
 * strlist strlist_range(const strlist* sl, size_t from, size_t to);
 */
TEST(test_strlist_range) {
  stralloc sa;
  strlist sl, r;
  strlist_init(&sl, '\0');
  strlist_init(&r, '\0');

  strlist_pushm_internal(&sl, "abcdefg", "abcdef", "abcde", "abcd", "abc", "ab", "a", 0);

  r = strlist_range(&sl, 2, 5);

  ASSERT_EQ(3, strlist_count(&r));

  stralloc_init(&sa);

  strlist_joins(&r, &sa, ", ");

  ASSERT_EQ(0, !stralloc_equals(&sa, "abcde, abcd, abc"));
}

/*
 * int strlist_shift(strlist* sl, const char** strp);
 */
TEST(test_strlist_shift) {
  const char* s;
  stralloc sa;
  strlist sl;
  strlist_init(&sl, ',');

  strlist_push(&sl, "water");
  strlist_push(&sl, "soil");
  strlist_push(&sl, "air");
  strlist_push(&sl, "fire");

  stralloc_init(&sa);

  strlist_joins(&sl, &sa, ", ");
  ASSERT_EQ(0, !stralloc_equals(&sa, "water, soil, air, fire"));

  strlist_shift(&sl, &s);

  stralloc_zero(&sa);
  strlist_joins(&sl, &sa, ", ");

  ASSERT_EQ(0, !stralloc_equals(&sa, "soil, air, fire"));
  ASSERT_EQ(0, !str_equal(s, "water"));

  strlist_shift(&sl, &s);

  stralloc_zero(&sa);
  strlist_joins(&sl, &sa, ", ");

  ASSERT_EQ(0, !stralloc_equals(&sa, "air, fire"));
  ASSERT_EQ(0, !str_equal(s, "soil"));
}

/*
 * int strlist_shift_n(strlist* sl, size_t i);
 */
TEST(test_strlist_shift_n) {
  const char* s;
  stralloc sa;
  strlist sl;
  strlist_init(&sl, ',');

  strlist_push(&sl, "water");
  strlist_push(&sl, "soil");
  strlist_push(&sl, "air");
  strlist_push(&sl, "fire");

  stralloc_init(&sa);

  strlist_joins(&sl, &sa, ", ");
  ASSERT_EQ(0, !stralloc_equals(&sa, "water, soil, air, fire"));

  strlist_shift_n(&sl, 2);

  stralloc_zero(&sa);
  strlist_joins(&sl, &sa, ", ");
  ASSERT_EQ(0, !stralloc_equals(&sa, "air, fire"));

  strlist_shift_n(&sl, 1);

  stralloc_zero(&sa);
  strlist_joins(&sl, &sa, ", ");
  ASSERT_EQ(0, !stralloc_equals(&sa, "fire"));
}

/*
 * size_t strlist_sort(strlist* sl);
 */
TEST(test_strlist_sort) {
  const char* s;
  stralloc sa;
  strlist sl;
  strlist_init(&sl, ',');

  strlist_push(&sl, "water");
  strlist_push(&sl, "soil");
  strlist_push(&sl, "air");
  strlist_push(&sl, "fire");

  strlist_sort(&sl, 0);

  stralloc_init(&sa);

  strlist_joins(&sl, &sa, ", ");

  buffer_puts(buffer_1, "stralloc: ");
  buffer_putsa(buffer_1, &sl.sa);
  buffer_putnlflush(buffer_1);
}

/*
 * char* *strlist_to_argv(const strlist* sl);
 */
TEST(test_strlist_to_argv) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_unshift(strlist* sl, const char* s);
 */
TEST(test_strlist_unshift) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/* true when the list holds exactly the n strings of v */
static int
list_is(const strlist* sl, size_t n, const char* const* v) {
  size_t i;

  if(strlist_count(sl) != n)
    return 0;
  for(i = 0; i < n; ++i) {
    size_t len;
    const char* x = strlist_at_n(sl, i, &len);

    if(!x || len != str_len(v[i]) || !byte_equal(x, len, v[i]))
      return 0;
  }
  return 1;
}

static int
longer_than_two(const char* s, size_t n) {
  (void)s;
  return n > 2;
}

static int
cmp_cstr(const void* a, const void* b) {
  return str_diff(*(const char* const*)a, *(const char* const*)b);
}

static ssize_t
discard(int fd, const void* x, size_t n, void* b) {
  (void)fd;
  (void)x;
  (void)b;
  return (ssize_t)n;
}

/*
 * push, pushb, at, at_n, contains, index_of, pos, count_pred
 */
TEST(test_strlist_access) {
  strlist sl;
  size_t n;
  const char* p;
  static const char* const three[] = {"one", "two", "three"};

  strlist_init(&sl, '\0');
  strlist_push(&sl, "one");
  strlist_push(&sl, "two");
  strlist_pushb(&sl, "threeXX", 5);
  ASSERT_EQ(1, list_is(&sl, 3, three));
  ASSERT_STR_EQUAL("two", strlist_at(&sl, 1));
  p = strlist_at_n(&sl, 2, &n);
  ASSERT_EQ(5, n);
  ASSERT_EQ(0, byte_diff(p, n, "three"));
  ASSERT_EQ(1, strlist_at(&sl, 9) == NULL);

  ASSERT_EQ(1, strlist_contains(&sl, "two"));
  ASSERT_EQ(0, strlist_containsb(&sl, "thr", 3)); /* whole elements only */
  ASSERT_EQ(1, strlist_containsb(&sl, "three", 5));
  ASSERT_EQ(4, strlist_index_of(&sl, "two")); /* a byte offset */
  ASSERT_EQ(8, strlist_index_of_b(&sl, "three", 5));
  ASSERT_EQ(-1, strlist_index_of(&sl, "zzz"));
  ASSERT_EQ(8, strlist_pos(&sl, 2));
  ASSERT_EQ(-1, strlist_pos(&sl, 3));
  ASSERT_EQ(3, strlist_count_pred(&sl, longer_than_two));
  stralloc_free(&sl.sa);
}

/*
 * strlist_pos must count the real length of every element (it measured every
 * element like the first one)
 */
TEST(test_strlist_pos_uneven) {
  strlist sl;

  strlist_init(&sl, '\0');
  strlist_push(&sl, "a");
  strlist_push(&sl, "bbbbbb");
  strlist_push(&sl, "cc");
  strlist_push(&sl, "d");
  ASSERT_EQ(0, strlist_pos(&sl, 0));
  ASSERT_EQ(2, strlist_pos(&sl, 1));
  ASSERT_EQ(9, strlist_pos(&sl, 2));
  ASSERT_EQ(12, strlist_pos(&sl, 3));
  stralloc_free(&sl.sa);
}

TEST(test_strlist_unique_and_front) {
  strlist sl;
  static const char* const expect[] = {"::", "minus", "zero", "one", "four"};
  static const char* const unique[] = {"one", "four"};

  strlist_init(&sl, '\0');
  strlist_push(&sl, "one");
  ASSERT_EQ(0, strlist_push_unique(&sl, "one"));
  ASSERT_EQ(1, strlist_pushb_unique(&sl, "fourXX", 4));
  ASSERT_EQ(0, strlist_pushb_unique(&sl, "four", 4));
  ASSERT_EQ(1, list_is(&sl, 2, unique));

  strlist_unshift(&sl, "zero");
  strlist_unshiftb(&sl, "minusXX", 5);
  strlist_prependb(&sl, "::", 2);
  ASSERT_EQ(1, list_is(&sl, 5, expect));
  strlist_prepends(&sl, ">");
  ASSERT_STR_EQUAL(">", strlist_at(&sl, 0));
  stralloc_free(&sl.sa);
}

TEST(test_strlist_remove) {
  strlist sl;
  static const char* const all[] = {"a", "b", "c", "d"};
  static const char* const no_b[] = {"a", "c", "d"};
  static const char* const no_d[] = {"a", "c"};
  static const char* const only_c[] = {"c"};

  strlist_init(&sl, '\0');
  strlist_push(&sl, "a");
  strlist_push(&sl, "b");
  strlist_push(&sl, "c");
  strlist_push(&sl, "d");
  ASSERT_EQ(1, list_is(&sl, 4, all));
  ASSERT_EQ(1, strlist_removes(&sl, "b"));
  ASSERT_EQ(1, list_is(&sl, 3, no_b));
  ASSERT_EQ(1, strlist_removeb(&sl, "dXX", 1)); /* the LAST element: no empty one left behind */
  ASSERT_EQ(1, list_is(&sl, 2, no_d));
  ASSERT_EQ(1, strlist_remove_at(&sl, 0)); /* by element number */
  ASSERT_EQ(1, list_is(&sl, 1, only_c));
  ASSERT_EQ(0, strlist_removes(&sl, "zz"));
  ASSERT_EQ(0, strlist_remove_at(&sl, 5));
  ASSERT_EQ(1, strlist_pop(&sl)); /* a single element can be popped */
  ASSERT_EQ(0, strlist_count(&sl));
  ASSERT_EQ(0, strlist_pop(&sl)); /* an empty list cannot */
  stralloc_free(&sl.sa);
}

TEST(test_strlist_trunc_shift) {
  strlist sl;
  const char* sp = NULL;
  static const char* const two[] = {"a", "b"};

  strlist_init(&sl, '\0');
  strlist_push(&sl, "a");
  strlist_push(&sl, "b");
  strlist_push(&sl, "c");
  ASSERT_EQ(2, strlist_trunc(&sl, 2));
  ASSERT_EQ(1, list_is(&sl, 2, two));
  ASSERT_EQ(1, strlist_shift_n(&sl, 1));
  /* the last element is not NUL terminated: use the length */
  ASSERT_EQ(1, strlist_count(&sl) == 1 && sl.sa.len == 1 && sl.sa.s[0] == 'b');
  ASSERT_EQ(1, strlist_shift(&sl, &sp) > 0);
  ASSERT_EQ(0, strlist_count(&sl));
  stralloc_free(&sl.sa);
}

TEST(test_strlist_from_join) {
  strlist sl, tok;
  stralloc sa;
  const char* v[] = {"p", "q", "r"};
  char** argv;
  static const char* const abc[] = {"a", "b", "", "c"};

  stralloc_init(&sa);
  strlist_init(&sl, '\0');
  strlist_init(&tok, '\0');
  strlist_fromv(&sl, v, 3);
  ASSERT_EQ(1, list_is(&sl, 3, v));
  argv = strlist_to_argv(&sl);
  ASSERT_STR_EQUAL("q", argv[1]);
  ASSERT_EQ(1, argv[3] == NULL);
  free(argv);

  strlist_join(&sl, &sa, ',');
  ASSERT_EQ(1, sa.len == 5 && byte_equal(sa.s, 5, "p,q,r"));
  stralloc_zero(&sa);
  strlist_joins(&sl, &sa, "; ");
  ASSERT_EQ(1, sa.len == 7 && byte_equal(sa.s, 7, "p; q; r"));
  stralloc_zero(&sa);
  ASSERT_EQ(1, strlist_copyat(&sl, 1, &sa));
  ASSERT_EQ(1, sa.len == 1 && sa.s[0] == 'q');
  stralloc_zero(&sa);
  ASSERT_EQ(1, strlist_at_sa(&sl, &sa, 2));
  ASSERT_EQ('r', sa.s[0]);

  strlist_froms(&tok, "a:b:c", ':');
  ASSERT_EQ(3, strlist_count(&tok));
  strlist_push(&tok, "d"); /* froms leaves a list that push continues correctly */
  ASSERT_EQ(4, strlist_count(&tok));
  stralloc_free(&tok.sa);
  strlist_init(&tok, '\0');
  strlist_fromb(&tok, "a--b--c", 7, "--"); /* the delimiter is a string */
  ASSERT_EQ(3, strlist_count(&tok));
  stralloc_free(&tok.sa);
  strlist_init(&tok, ' ');
  strlist_push_tokens(&tok, "one two  three", " "); /* the delimiter is a set of characters */
  ASSERT_EQ(1, strlist_count(&tok) >= 3);
  (void)abc;
  stralloc_free(&tok.sa);
  stralloc_free(&sl.sa);
  stralloc_free(&sa);
}

TEST(test_strlist_sa_variants) {
  strlist sl;
  stralloc sa;

  strlist_init(&sl, '\0');
  stralloc_init(&sa);
  stralloc_copys(&sa, "sa1");
  ASSERT_EQ(1, strlist_push_sa(&sl, &sa));
  ASSERT_EQ(1, strlist_contains_sa(&sl, &sa));
  ASSERT_EQ(0, strlist_push_unique_sa(&sl, &sa));
  ASSERT_EQ(1, strlist_append_sa(&sl, &sa) != 0);
  ASSERT_EQ(1, strlist_prepend_sa(&sl, &sa) != 0);
  ASSERT_EQ(1, strlist_count(&sl) >= 2);
  stralloc_free(&sl.sa);
  stralloc_free(&sa);
}

TEST(test_strlist_sort_slice) {
  strlist s, out;
  static const char* const sorted[] = {"apple", "fig", "pear"};
  static const char* const fig[] = {"fig"};
  static const char* const last_two[] = {"fig", "pear"};
  static const char* const first_two[] = {"apple", "fig"};
  strlist r;

  strlist_init(&s, '\0');
  strlist_push(&s, "pear");
  strlist_push(&s, "apple");
  strlist_push(&s, "fig");
  ASSERT_EQ(3, strlist_sort(&s, cmp_cstr));
  ASSERT_EQ(1, list_is(&s, 3, sorted));

  strlist_init(&out, '\0');
  strlist_slice(&out, &s, 1, 2); /* end is exclusive */
  ASSERT_EQ(1, list_is(&out, 1, fig));
  stralloc_free(&out.sa);
  strlist_init(&out, '\0');
  strlist_slice(&out, &s, -2, 3); /* negative: from the end */
  ASSERT_EQ(1, list_is(&out, 2, last_two));
  stralloc_free(&out.sa);
  strlist_init(&out, '\0');
  strlist_slice(&out, &s, 0, -1);
  ASSERT_EQ(1, list_is(&out, 2, first_two));
  stralloc_free(&out.sa);
  strlist_init(&out, '\0');
  strlist_slice(&out, &s, 2, 99); /* clipped */
  ASSERT_EQ(1, strlist_count(&out) == 1);
  stralloc_free(&out.sa);

  r = strlist_range(&s, 1, 3); /* a view into s, shares its memory */
  ASSERT_EQ(2, strlist_count(&r));
  stralloc_free(&s.sa);
}

TEST(test_strlist_set_operations) {
  strlist a, b, u, i, d1, d2, df;
  static const char* const exp_u[] = {"apple", "fig", "pear", "kiwi"};
  static const char* const exp_i[] = {"fig"};
  static const char* const left[] = {"apple", "pear"};
  static const char* const right[] = {"kiwi"};

  strlist_init(&a, '\0');
  strlist_init(&b, '\0');
  strlist_init(&u, '\0');
  strlist_init(&i, '\0');
  strlist_init(&d1, '\0');
  strlist_init(&d2, '\0');
  strlist_init(&df, '\0');
  strlist_push(&a, "apple");
  strlist_push(&a, "fig");
  strlist_push(&a, "pear");
  strlist_push(&b, "fig");
  strlist_push(&b, "kiwi");

  strlist_union(&a, &b, &u);
  ASSERT_EQ(1, list_is(&u, 4, exp_u));
  strlist_intersection(&a, &b, &i);
  ASSERT_EQ(1, list_is(&i, 1, exp_i));
  strlist_difference(&a, &b, &d1, &d2);
  ASSERT_EQ(1, list_is(&d1, 2, left));
  ASSERT_EQ(1, list_is(&d2, 1, right));
  ASSERT_EQ(2, strlist_diff(&a, &b, &df));
  ASSERT_EQ(1, list_is(&df, 2, left));

  /* sub removes one occurrence per element, remove_all every occurrence */
  strlist_cat(&a, &b); /* apple fig pear fig kiwi */
  ASSERT_EQ(5, strlist_count(&a));
  ASSERT_EQ(2, strlist_sub(&a, &b)); /* one fig, kiwi */
  ASSERT_EQ(3, strlist_count(&a));
  ASSERT_EQ(1, strlist_remove_all(&a, &b)); /* the other fig */
  ASSERT_EQ(1, list_is(&a, 2, left));
  strlist_cat_unique(&a, &b);
  ASSERT_EQ(4, strlist_count(&a));
  strlist_cat_unique(&a, &b); /* nothing new */
  ASSERT_EQ(4, strlist_count(&a));

  stralloc_free(&a.sa);
  stralloc_free(&b.sa);
  stralloc_free(&u.sa);
  stralloc_free(&i.sa);
  stralloc_free(&d1.sa);
  stralloc_free(&d2.sa);
  stralloc_free(&df.sa);
}

TEST(test_strlist_copy_filter_match) {
  strlist s, cp, m, nm;
  static const char* const matching[] = {"apple", "pear"};

  strlist_init(&s, '\0');
  strlist_init(&cp, '\0');
  strlist_init(&m, '\0');
  strlist_init(&nm, '\0');
  strlist_push(&s, "apple");
  strlist_push(&s, "fig");
  strlist_push(&s, "pear");
  strlist_copy(&cp, &s);
  ASSERT_EQ(3, strlist_count(&cp));
  ASSERT_EQ(1, cp.sa.s != s.sa.s); /* deep copy */
  strlist_filter(&s, &m, &nm, "*p*");
  ASSERT_EQ(1, list_is(&m, 2, matching));
  ASSERT_EQ(1, strlist_count(&nm));
  ASSERT_EQ(1, strlist_match(&s, "f*", 0)); /* index of the first match */
  ASSERT_EQ(-1, strlist_match(&s, "zz*", 0));
  stralloc_free(&s.sa);
  stralloc_free(&cp.sa);
  stralloc_free(&m.sa);
  stralloc_free(&nm.sa);
}

TEST(test_strlist_splice_replace) {
  strlist s;
  static const char* const a1[] = {"X", "pear"};
  static const char* const a2[] = {"YY", "pear"};
  static const char* const a3[] = {"YY", "mid", "pear"};

  strlist_init(&s, '\0');
  strlist_push(&s, "apple");
  strlist_push(&s, "pear");
  ASSERT_EQ(1, strlist_splice(&s, 0, 1, "X", 1)); /* positions are element numbers */
  ASSERT_EQ(1, list_is(&s, 2, a1));
  ASSERT_EQ(1, strlist_replaceb(&s, "X", 1, "YY", 2));
  ASSERT_EQ(1, list_is(&s, 2, a2));
  ASSERT_EQ(0, strlist_replaceb(&s, "nope", 4, "z", 1));
  ASSERT_EQ(1, strlist_splice(&s, 1, 0, "mid", 3)); /* insert only */
  ASSERT_EQ(1, list_is(&s, 3, a3));
  ASSERT_EQ(1, strlist_splice(&s, 1, 1, NULL, 0)); /* delete only */
  ASSERT_EQ(2, strlist_count(&s));
  stralloc_free(&s.sa);
}

TEST(test_strlist_quoted) {
  strlist q, s;
  stralloc sa;

  strlist_init(&q, '\0');
  strlist_init(&s, '\0');
  stralloc_init(&sa);
  strlist_fromq(&q, "a 'b c' d", 9, " ", "'");
  ASSERT_EQ(3, strlist_count(&q));
  ASSERT_STR_EQUAL("b c", strlist_at(&q, 1));
  strlist_fromsq(&s, "a \"b c\" d", " ", "\"");
  ASSERT_EQ(3, strlist_count(&s));
  strlist_joinq(&q, &sa, ',', '"');
  ASSERT_EQ(1, sa.len >= 5);
  stralloc_free(&q.sa);
  stralloc_free(&s.sa);
  stralloc_free(&sa);
}

TEST(test_strlist_output) {
  strlist s;
  buffer b;
  char mem[512];
  const char* names[] = {"n0", "n1", "n2", 0};

  strlist_init(&s, '\0');
  strlist_push(&s, "apple");
  strlist_push(&s, "fig");
  buffer_init(&b, (buffer_op_ptr)0, -1, mem, sizeof mem);
  buffer_putsl(&b, &s, ", ");
  ASSERT_EQ(10, b.p);
  ASSERT_EQ(0, byte_diff(mem, 10, "apple, fig"));
  b.p = 0;
  buffer_putslq(&b, &s, ',', '"');
  ASSERT_EQ(1, b.p >= 9);

  buffer_init(&b, (buffer_op_ptr)discard, -1, mem, sizeof mem);
  strlist_dump(&b, &s); /* ends with a flush */
  strlist_dump_named(&b, &s, names);
  ASSERT_EQ(0, b.p);
  stralloc_free(&s.sa);
}

/* regression: stralloc_trunc used to leave sa.a describing the old block */
TEST(test_stralloc_trunc_then_append) {
  stralloc sa;
  int i;

  stralloc_init(&sa);
  stralloc_copys(&sa, "0123456789abcdef0123456789abcdef");
  ASSERT_EQ(1, stralloc_trunc(&sa, 1));
  for(i = 0; i < 100; ++i)
    stralloc_catc(&sa, 'x'); /* valgrind: no write past the block */
  ASSERT_EQ(101, sa.len);
  stralloc_free(&sa);
}

#define RUN_STRLIST_TESTS() \
  RUN(test_strlist_access); \
  RUN(test_strlist_pos_uneven); \
  RUN(test_strlist_unique_and_front); \
  RUN(test_strlist_remove); \
  RUN(test_strlist_trunc_shift); \
  RUN(test_strlist_from_join); \
  RUN(test_strlist_sa_variants); \
  RUN(test_strlist_sort_slice); \
  RUN(test_strlist_set_operations); \
  RUN(test_strlist_copy_filter_match); \
  RUN(test_strlist_splice_replace); \
  RUN(test_strlist_quoted); \
  RUN(test_strlist_output); \
  RUN(test_stralloc_trunc_then_append); \
  RUN(test_strlist_at); \
  RUN(test_strlist_at_n); \
  RUN(test_strlist_at_sa); \
  RUN(test_strlist_cat); \
  RUN(test_strlist_contains); \
  RUN(test_strlist_contains_sa); \
  RUN(test_strlist_count); \
  RUN(test_strlist_dump); \
  RUN(test_strlist_froms); \
  RUN(test_strlist_fromb); \
  RUN(test_strlist_index_of); \
  RUN(test_strlist_join); \
  RUN(test_strlist_joins); \
  RUN(test_strlist_push); \
  RUN(test_strlist_push_sa); \
  RUN(test_strlist_push_tokens); \
  RUN(test_strlist_push_unique); \
  RUN(test_strlist_push_unique_sa); \
  RUN(test_strlist_pushb); \
  RUN(test_strlist_pushm); \
  RUN(test_strlist_range); \
  RUN(test_strlist_shift); \
  RUN(test_strlist_shift_n); \
  RUN(test_strlist_sort); \
  RUN(test_strlist_to_argv); \
  RUN(test_strlist_unshift)

TESTS(strlist) { RUN_STRLIST_TESTS(); }
