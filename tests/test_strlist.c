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
  stralloc sa;
  strlist sl;
  strlist_init(&sl, '\0');
  strlist_push(&sl, "1");
  strlist_push(&sl, "12");
  strlist_push(&sl, "123");
  strlist_push(&sl, "1234");

  sa = strlist_at_sa(&sl, 0);
  ASSERT_EQ(0, !stralloc_equals(&sa, "1"));
  sa = strlist_at_sa(&sl, 1);
  ASSERT_EQ(0, !stralloc_equals(&sa, "12"));
  sa = strlist_at_sa(&sl, 2);
  ASSERT_EQ(0, !stralloc_equals(&sa, "123"));
  sa = strlist_at_sa(&sl, 3);
  ASSERT_EQ(0, !stralloc_equals(&sa, "1234"));
}

/*
 * int strlist_cat(strlist* sl, const strlist* l);
 */
TEST(cat) {
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
  
//ASSERT_EQ(0, !str_equal(strlist_at(&sl1, 2), "1234"));
  strlist_join(&sl1, &sa, '|');

  buffer_putsa(buffer_1, &sa);
  buffer_putnlflush(buffer_1);
  
  ASSERT_EQ(0, !stralloc_equals(&sa, "blah|l33t|1234|8192"));
  
  ASSERT_EQ(4, strlist_count(&sl1));

}

/*
 * int strlist_contains(strlist* sl, const char* s);
 */
TEST(contains) {
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
TEST(contains_sa) {
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
  strlist_froms(&sl, "blah|l33t|1234|8192", '|');

  ASSERT_EQ(4, strlist_count(&sl));

  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 0), "blah"));
  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 1), "l33t"));
  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 2), "1234"));
  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 3), "8192"));
}

/*
 * int64 strlist_index_of(strlist* sl, const char* str);
 */
TEST(index_of) {
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
TEST(join) {
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
TEST(joins) {
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
TEST(push) {
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
TEST(push_sa) {
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
TEST(push_tokens) {
  strlist sl;
  strlist_init(&sl, '\0');
  
  strlist_push_tokens(&sl, "blah, l33t, 1234, 8192", ", ");

  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 3), "8192"));
  ASSERT_EQ(0, !str_equal(strlist_at(&sl, 2), "1234"));
}

/*
 * int strlist_push_unique(strlist* sl, const char* s);
 */
TEST(push_unique) {
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
TEST(push_unique_sa) {
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
TEST(pushb) {
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
TEST(pushm) {
  stralloc sa;
  strlist sl;
  stralloc_init(&sa);
  strlist_init(&sl, '\0');

  strlist_pushm(&sl, "abcd", "abc", "ab", "a");

  strlist_joins(&sl, &sa, " | ");
  
  ASSERT_EQ(0, !stralloc_equals(&sa, "abcd | abc | ab | a"));
}

/*
 * strlist strlist_range(const strlist* sl, size_t from, size_t to);
 */
TEST(range) {
  stralloc sa;
  strlist sl, r;
  strlist_init(&sl, '\0');
  strlist_init(&r, '\0');

  strlist_pushm(&sl, "abcdefg", "abcdef", "abcde", "abcd", "abc", "ab", "a");

  r = strlist_range(&sl, 2, 5);

  ASSERT_EQ(3, strlist_count(&r));
  
  stralloc_init(&sa);

  strlist_joins(&r, &sa, ", ");

  ASSERT_EQ(0, !stralloc_equals(&sa, "abcde, abcd, abc"));
}

/*
 * int strlist_shift(strlist* sl, const char** strp);
 */
TEST(shift) {
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
TEST(shift_n) {
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
TEST(sort) {
  const char* s;
  stralloc sa;
  strlist sl;
  strlist_init(&sl, ',');
  
  strlist_push(&sl, "water");
  strlist_push(&sl, "soil");
  strlist_push(&sl, "air");
  strlist_push(&sl, "fire");
  
 strlist_sort(&sl);

  stralloc_init(&sa);

  strlist_joins(&sl, &sa, ", ");

  buffer_puts(buffer_1, "stralloc: ");
  buffer_putsa(buffer_1, &sl.sa);
  buffer_putnlflush(buffer_1);
  
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
  RUN(pushm);
  RUN(range);
  RUN(shift);
  RUN(shift_n);
  RUN(sort);
  RUN(to_argv);
  RUN(unshift);
}
