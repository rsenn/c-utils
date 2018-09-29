#include "lib/unit_test.h"
#include "lib/strlist.h"


/*
 * size_t strlist_count(const strlist* sl);
 */
TEST(count) {
  strlist sl;
  strlist_init(&sl, '\0');
  strlist_push(&sl, "1");
  strlist_push(&sl, "2");
  strlist_push(&sl, "3");
  strlist_push(&sl, "4");

  ASSERT_EQ(4, strlist_count(&sl));
}

/*
 * char* strlist_at(const strlist* sl, size_t i);
 */
TEST(at) {
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
TEST(at_n) {
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
TEST(at_sa) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_cat(strlist* sl, const strlist* l);
 */
TEST(cat) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_contains(strlist* sl, const char* s);
 */
TEST(contains) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_contains_sa(strlist* sl, const stralloc* sa);
 */
TEST(contains_sa) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * void strlist_dump(buffer* out, const strlist* sl);
 */
TEST(dump) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * void strlist_froms(strlist* sl, const char* s, char delim);
 */
TEST(froms) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int64 strlist_index_of(strlist* sl, const char* str);
 */
TEST(index_of) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * void strlist_join(const strlist* sl, stralloc* sa, char delim);
 */
TEST(join) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * void strlist_joins(const strlist* sl, stralloc* sa, const char* delim);
 */
TEST(joins) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_push(strlist* sl, const char* s);
 */
TEST(push) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_push_sa(strlist* sl, const stralloc* sa);
 */
TEST(push_sa) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * void strlist_push_tokens(strlist* sl, const char* s, const char* delim);
 */
TEST(push_tokens) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_push_unique(strlist* sl, const char* s);
 */
TEST(push_unique) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_push_unique_sa(strlist* sl, const stralloc* sa);
 */
TEST(push_unique_sa) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_pushb(strlist* sl, const char* s, size_t n);
 */
TEST(pushb) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_pushm_internal(strlist* sl, ...);
 */
TEST(pushm_internal) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_pushsa(strlist* sl, const stralloc* sa);
 */
TEST(pushsa) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * strlist strlist_range(const strlist* sl, size_t from, size_t to);
 */
TEST(range) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_shift(strlist* sl, const char** strp);
 */
TEST(shift) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_shift_n(strlist* sl, size_t i);
 */
TEST(shift_n) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * size_t strlist_sort(strlist* sl);
 */
TEST(sort) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * char* *strlist_to_argv(const strlist* sl);
 */
TEST(to_argv) {
  strlist sl;
  strlist_init(&sl, '\0');
}

/*
 * int strlist_unshift(strlist* sl, const char* s);
 */
TEST(unshift) {
  strlist sl;
  strlist_init(&sl, '\0');
}

START() {
  RUN(at);
  RUN(at_n);
  RUN(at_sa);
  RUN(cat);
  RUN(contains);
  RUN(contains_sa);
  RUN(count);
  RUN(dump);
  RUN(froms);
  RUN(index_of);
  RUN(join);
  RUN(joins);
  RUN(push);
  RUN(push_sa);
  RUN(push_tokens);
  RUN(push_unique);
  RUN(push_unique_sa);
  RUN(pushb);
  RUN(pushm_internal);
  RUN(pushsa);
  RUN(range);
  RUN(shift);
  RUN(shift_n);
  RUN(sort);
  RUN(to_argv);
  RUN(unshift);
}
