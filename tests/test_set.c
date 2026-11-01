#include "unit_test.h"
#include "../lib/set.h"
#include "../lib/str.h"
#include "../lib/byte.h"
#include "../lib/stralloc.h"
#include "../lib/buffer.h"
#include <stdio.h>
#include <stdlib.h>

/* every value in one bucket: all the code for chains runs */
static uint32
same_hash(const void* x, size_t n) {
  (void)x;
  (void)n;
  return 5;
}

/* the values in the order of insertion, separated by commas */
static void
dump(const set_t* s, char* out) {
  bucket_t* b;
  size_t i = 0;

  out[0] = '\0';
  for(b = s->list; b; b = b->list_next) {
    if(i++)
      str_copy(out + str_len(out), ",");
    str_copyn(out + str_len(out), b->value, b->size);
  }
}

/* the same, found by walking the buckets backwards */
static void
dump_rev(const set_t* s, char* out) {
  bucket_t* b;
  size_t i = 0;

  out[0] = '\0';
  for(b = s->last; b; b = b->list_prev) {
    if(i++)
      str_copy(out + str_len(out), ",");
    str_copyn(out + str_len(out), b->value, b->size);
  }
}

/*
 * void set_init(set_t*, hash);
 * int set_add(set_t*, const void*, size_t);
 * int set_adds(set_t*, const char*);
 * int set_has(const set_t*, const void*, size_t);
 * int set_has_s(const set_t*, const char*);
 */
TEST(test_set_add_has) {
  set_t s = SET();
  char out[64];

  /* a set that was never started is empty */
  ASSERT_EQ(0, set_has_s(&s, "a"));
  ASSERT_EQ(0, set_size(&s));

  ASSERT_EQ(1, set_adds(&s, "one"));
  ASSERT_EQ(1, set_adds(&s, "two"));
  ASSERT_EQ(1, set_add(&s, "three!", 5)); /* 5 bytes: three */
  ASSERT_EQ(3, set_size(&s));

  /* a value that is there says 2 and is not added again */
  ASSERT_EQ(2, set_adds(&s, "two"));
  ASSERT_EQ(3, set_size(&s));

  ASSERT_NE(0, set_has_s(&s, "one"));
  ASSERT_NE(0, set_has_s(&s, "three"));
  ASSERT_EQ(0, set_has_s(&s, "four"));
  ASSERT_EQ(0, set_has_s(&s, "thre"));
  ASSERT_EQ(0, set_has_s(&s, "threee"));
  ASSERT_NE(0, set_has(&s, "two", 3));
  ASSERT_EQ(0, set_has(&s, "two", 2));

  dump(&s, out);
  ASSERT_STR_EQUAL("one,two,three", out);
  dump_rev(&s, out);
  ASSERT_STR_EQUAL("three,two,one", out);

  set_free(&s);
}

/* the empty string and binary values */
TEST(test_set_values) {
  set_t s = SET();

  ASSERT_EQ(1, set_add(&s, "", 0));
  ASSERT_EQ(2, set_add(&s, "", 0));
  ASSERT_EQ(1, set_add(&s, "a\0b", 3));
  ASSERT_EQ(1, set_add(&s, "a\0c", 3));
  ASSERT_EQ(3, set_size(&s));
  ASSERT_NE(0, set_has(&s, "a\0b", 3));
  ASSERT_EQ(0, set_has(&s, "a\0d", 3));
  ASSERT_NE(0, set_has(&s, "", 0));
  set_free(&s);
}


/*
 * values that do not fit one slot: chains
 */
TEST(test_set_collisions) {
  set_t s = SET();
  char out[64];

  set_init(&s);
  s.hash_fp = same_hash;
  ASSERT_EQ(1, set_adds(&s, "a"));
  ASSERT_EQ(1, set_adds(&s, "b"));
  ASSERT_EQ(1, set_adds(&s, "c"));
  ASSERT_EQ(1, set_adds(&s, "d"));
  ASSERT_EQ(2, set_adds(&s, "c"));
  ASSERT_EQ(4, set_size(&s));
  ASSERT_EQ(3, s.overflow);

  ASSERT_NE(0, set_has_s(&s, "a"));
  ASSERT_NE(0, set_has_s(&s, "d"));
  ASSERT_EQ(0, set_has_s(&s, "e"));

  dump(&s, out);
  ASSERT_STR_EQUAL("a,b,c,d", out);
  set_free(&s);
}


/* the slot of the array and the buckets behind it */
TEST(test_set_delete_chain) {
  set_t s = SET();
  char out[64];

  set_init(&s);
  s.hash_fp = same_hash;
  set_adds(&s, "a");
  set_adds(&s, "b");
  set_adds(&s, "c");
  set_adds(&s, "d");
  ASSERT_EQ(3, s.overflow);

  /* the one in the slot: the next in the chain takes its place */
  ASSERT_EQ(1, set_delete(&s, "a", 1));
  ASSERT_EQ(3, set_size(&s));
  ASSERT_EQ(2, s.overflow);
  ASSERT_EQ(0, set_has_s(&s, "a"));
  ASSERT_NE(0, set_has_s(&s, "b"));
  ASSERT_NE(0, set_has_s(&s, "c"));
  ASSERT_NE(0, set_has_s(&s, "d"));
  dump(&s, out);
  ASSERT_STR_EQUAL("b,c,d", out);
  dump_rev(&s, out);
  ASSERT_STR_EQUAL("d,c,b", out);

  /* one in the middle of the chain, and the last one */
  ASSERT_EQ(1, set_delete(&s, "c", 1));
  ASSERT_EQ(1, set_delete(&s, "d", 1));
  ASSERT_EQ(0, s.overflow);
  dump(&s, out);
  ASSERT_STR_EQUAL("b", out);

  ASSERT_EQ(1, set_delete(&s, "b", 1));
  ASSERT_EQ(0, set_size(&s));
  set_free(&s);
}

/*
 * void set_clear(set_t*);
 * void set_free(set_t*);
 */
TEST(test_set_clear_free) {
  set_t s = SET();
  int i;
  char v[16];

  for(i = 0; i < 100; i++) {
    snprintf(v, sizeof(v), "v%d", i);
    set_adds(&s, v);
  }
  ASSERT_EQ(100, set_size(&s));

  set_clear(&s);
  ASSERT_EQ(0, set_size(&s));
  ASSERT_EQ(NULL, s.list);
  ASSERT_EQ(0, set_has_s(&s, "v1"));

  /* it keeps working after it was cleared */
  ASSERT_EQ(1, set_adds(&s, "v1"));
  ASSERT_EQ(1, set_size(&s));

  set_free(&s);
  ASSERT_EQ(NULL, s.array);
  ASSERT_EQ(0, set_size(&s));
  ASSERT_EQ(0, set_has_s(&s, "v1"));

  /* free twice, clear when freed, free of nothing */
  set_free(&s);
  set_clear(&s);
  set_free(NULL);
  set_clear(NULL);

  /* a freed set is a new one */
  ASSERT_EQ(1, set_adds(&s, "again"));
  ASSERT_NE(0, set_has_s(&s, "again"));
  set_free(&s);
}

/* the table grows, nothing gets lost and the order stays */




static int
starts_with_a(const char* x, size_t n) {
  return n > 0 && x[0] == 'a';
}





/*
 * int set_insert(set_t*, const void*, size_t);
 */
TEST(test_set_insert) {
  set_t s = SET();
  char out[64];

  ASSERT_EQ(SET_ADDED, set_insert(&s, "a", 1));
  ASSERT_EQ(SET_ADDED, set_insert(&s, "b", 1));
  ASSERT_EQ(SET_ADDED, set_insert(&s, "c", 1));
  ASSERT_EQ(SET_EXISTS, set_insert(&s, "a", 1));

  /* inserted values go to the front, added ones to the back */
  ASSERT_EQ(SET_ADDED, set_adds(&s, "z"));
  dump(&s, out);
  ASSERT_STR_EQUAL("c,b,a,z", out);
  dump_rev(&s, out);
  ASSERT_STR_EQUAL("z,a,b,c", out);
  set_free(&s);
}

/*
 * int set_delete(set_t*, const void*, size_t);
 */
TEST(test_set_delete) {
  set_t s = SET();
  char out[64];

  ASSERT_EQ(0, set_delete(&s, "a", 1));

  set_adds(&s, "a");
  set_adds(&s, "b");
  set_adds(&s, "c");

  ASSERT_EQ(0, set_delete(&s, "x", 1));
  ASSERT_EQ(3, set_size(&s));

  ASSERT_EQ(1, set_delete(&s, "b", 1));
  ASSERT_EQ(2, set_size(&s));
  ASSERT_EQ(0, set_has_s(&s, "b"));
  dump(&s, out);
  ASSERT_STR_EQUAL("a,c", out);
  dump_rev(&s, out);
  ASSERT_STR_EQUAL("c,a", out);

  ASSERT_EQ(0, set_delete(&s, "b", 1));
  ASSERT_EQ(1, set_delete(&s, "a", 1));
  dump(&s, out);
  ASSERT_STR_EQUAL("c", out);
  ASSERT_EQ(1, set_delete(&s, "c", 1));
  ASSERT_EQ(0, set_size(&s));
  ASSERT_EQ(NULL, s.list);
  ASSERT_EQ(NULL, s.last);

  /* and it can be filled again */
  ASSERT_EQ(SET_ADDED, set_adds(&s, "z"));
  ASSERT_NE(0, set_has_s(&s, "z"));
  set_free(&s);
}

/* the table grows, nothing gets lost, the order stays */
TEST(test_set_many) {
  set_t s = SET();
  char v[16];
  int i, n;
  const char* x;
  size_t len;
  set_iterator_t it;

  for(i = 0; i < 5000; i++) {
    snprintf(v, sizeof(v), "value-%d", i);
    ASSERT_EQ(SET_ADDED, set_adds(&s, v));
  }
  ASSERT_EQ(5000, set_size(&s));
  ASSERT_GT(s.len, 16);

  for(i = 0; i < 5000; i++) {
    snprintf(v, sizeof(v), "value-%d", i);
    ASSERT_NE(0, set_has_s(&s, v));
  }
  ASSERT_STR_EQUAL("value-0", set_first(&s, NULL));
  ASSERT_STR_EQUAL("value-4999", set_last(&s, NULL));

  for(i = 0; i < 5000; i += 2) {
    snprintf(v, sizeof(v), "value-%d", i);
    ASSERT_EQ(1, set_delete(&s, v, str_len(v)));
  }
  ASSERT_EQ(2500, set_size(&s));
  ASSERT_STR_EQUAL("value-1", set_first(&s, NULL));

  /* the walk follows the order of insertion */
  n = 0;
  i = 1;
  set_foreach(&s, it, x, len) {
    snprintf(v, sizeof(v), "value-%d", i);
    ASSERT_EQ(0, str_diff(v, x));
    i += 2;
    n++;
  }
  ASSERT_EQ(2500, n);
  set_free(&s);
}

/*
 * char* set_first(const set_t*, size_t*);
 * char* set_last(const set_t*, size_t*);
 */
TEST(test_set_first_last) {
  set_t s = SET();
  size_t n = 99;

  ASSERT_EQ(NULL, set_first(&s, &n));
  ASSERT_EQ(NULL, set_last(&s, &n));

  set_adds(&s, "only");
  ASSERT_STR_EQUAL("only", set_first(&s, &n));
  ASSERT_EQ(4, n);
  ASSERT_STR_EQUAL("only", set_last(&s, &n));

  set_adds(&s, "second");
  set_insert(&s, "zeroth", 6);
  ASSERT_STR_EQUAL("zeroth", set_first(&s, &n));
  ASSERT_EQ(6, n);
  ASSERT_STR_EQUAL("second", set_last(&s, &n));
  ASSERT_EQ(6, n);

  /* the size is optional */
  ASSERT_STR_EQUAL("zeroth", set_first(&s, NULL));

  set_delete(&s, "zeroth", 6);
  ASSERT_STR_EQUAL("only", set_first(&s, NULL));
  set_free(&s);
}

/*
 * iterators and the foreach macros
 */
TEST(test_set_iterator) {
  set_t s = SET();
  set_iterator_t it;
  size_t n;
  const char* x;
  int count = 0, sum = 0;

  ASSERT_EQ(0, set_iterator_init(&it, &s));
  ASSERT_EQ(NULL, set_iterator_value(&it, &n));
  ASSERT_EQ(0, set_iterator_next(&it));

  set_adds(&s, "1");
  set_adds(&s, "2");
  set_adds(&s, "3");
  set_adds(&s, "4");

  set_foreach(&s, it, x, n) {
    ASSERT_EQ(1, n);
    sum = sum * 10 + (x[0] - '0');
    count++;
  }
  ASSERT_EQ(4, count);
  ASSERT_EQ(1234, sum);

  /* by hand */
  ASSERT_NE(0, set_iterator_init(&it, &s));
  ASSERT_STR_EQUAL("1", set_iterator_value(&it, &n));
  ASSERT_NE(0, set_iterator_next(&it));
  ASSERT_STR_EQUAL("2", set_iterator_value(&it, NULL));
  set_iterator_next(&it);
  set_iterator_next(&it);
  ASSERT_STR_EQUAL("4", set_iterator_value(&it, NULL));
  ASSERT_EQ(0, set_iterator_next(&it));
  ASSERT_EQ(NULL, set_iterator_value(&it, NULL));
  /* at the end it stays there */
  ASSERT_EQ(0, set_iterator_next(&it));

  /* the buckets */
  {
    const bucket_t* b;
    char out[16];
    size_t len = 0;

    out[0] = '\0';
    set_foreach_ordered(&s, b, x, len) str_copyn(out + str_len(out), x, len);
    ASSERT_STR_EQUAL("1234", out);
  }

  it = set_iterator_new(&s);
  ASSERT_NE(NULL, set_iterator_value(&it, &n));
  set_free(&s);
}


/*
 * int set_cat / set_intersection / set_filter / set_filter_out
 */
TEST(test_set_combine) {
  set_t a = SET(), b = SET(), c = SET();
  char out[64];

  set_adds(&a, "a1");
  set_adds(&a, "a2");
  set_adds(&a, "both");
  set_adds(&b, "b1");
  set_adds(&b, "both");

  /* what is new in c is added, in the order of the source */
  ASSERT_EQ(3, set_cat(&c, &a));
  ASSERT_EQ(1, set_cat(&c, &b));
  ASSERT_EQ(0, set_cat(&c, &b));
  dump(&c, out);
  ASSERT_STR_EQUAL("a1,a2,both,b1", out);

  /* nothing to do with itself */
  ASSERT_EQ(0, set_cat(&c, &c));
  ASSERT_EQ(4, set_size(&c));

  set_clear(&c);
  ASSERT_EQ(1, set_intersection(&c, &a, &b));
  dump(&c, out);
  ASSERT_STR_EQUAL("both", out);

  set_clear(&c);
  ASSERT_EQ(2, set_filter(&a, &c, starts_with_a));
  dump(&c, out);
  ASSERT_STR_EQUAL("a1,a2", out);

  set_clear(&c);
  ASSERT_EQ(1, set_filter_out(&a, &c, starts_with_a));
  dump(&c, out);
  ASSERT_STR_EQUAL("both", out);

  /* with an empty set */
  set_clear(&c);
  ASSERT_EQ(0, set_intersection(&c, &a, &c));
  ASSERT_EQ(0, set_cat(&c, &c));

  set_free(&a);
  set_free(&b);
  set_free(&c);
}

/*
 * set_join, set_join_s, buffer_putset
 */
TEST(test_set_text) {
  set_t s = SET();
  stralloc sa;
  char* joined;

  stralloc_init(&sa);

  /* an empty set gives nothing */
  ASSERT_EQ(1, set_join(&s, ",", &sa));
  ASSERT_EQ(0, sa.len);

  set_adds(&s, "x");
  set_adds(&s, "y z");
  set_adds(&s, "w");

  ASSERT_EQ(1, set_join(&s, "+", &sa));
  ASSERT_EQ(7, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "x+y z+w"));

  joined = set_join_s(&s, ", ");
  ASSERT_NE(NULL, joined);
  ASSERT_STR_EQUAL("x, y z, w", joined);
  free(joined);

  {
    buffer b;
    char mem[64];

    buffer_init(&b, NULL, -1, mem, sizeof(mem));
    buffer_putset(&b, &s, ", ", 2);
    ASSERT_EQ(9, b.p);
    ASSERT_EQ(0, byte_diff(mem, 9, "x, y z, w"));
  }

  stralloc_free(&sa);
  set_free(&s);
}

/*
 * int set_addsa(set_t*, const stralloc*);
 * int set_has_sa(const set_t*, const stralloc*);
 */
TEST(test_set_stralloc) {
  set_t s = SET();
  stralloc a;

  stralloc_init(&a);
  stralloc_copys(&a, "key");

  ASSERT_EQ(SET_ADDED, set_addsa(&s, &a));
  ASSERT_EQ(SET_EXISTS, set_addsa(&s, &a));
  ASSERT_NE(0, set_has_sa(&s, &a));
  ASSERT_NE(0, set_has_s(&s, "key"));

  stralloc_copys(&a, "other");
  ASSERT_EQ(0, set_has_sa(&s, &a));
  ASSERT_EQ(1, set_size(&s));

  stralloc_free(&a);
  set_free(&s);
}

/*
 * values are copied: a changed original does not change the set
 */
TEST(test_set_copies) {
  set_t s = SET();
  char v[8] = "abc";

  set_adds(&s, v);
  v[0] = 'X';
  ASSERT_NE(0, set_has_s(&s, "abc"));
  ASSERT_EQ(0, set_has_s(&s, "Xbc"));
  ASSERT_STR_EQUAL("abc", set_first(&s, NULL));
  set_free(&s);
}

/*
 * void set_init(set_t*);
 */
TEST(test_set_init) {
  set_t s;

  byte_fill(&s, sizeof(s), 0xff);
  set_init(&s);
  ASSERT_EQ(0, set_size(&s));
  ASSERT_EQ(NULL, s.array);
  ASSERT_EQ(NULL, s.list);
  ASSERT_EQ(0, set_has_s(&s, "x"));
  ASSERT_EQ(SET_ADDED, set_adds(&s, "x"));
  set_free(&s);
}

#define RUN_SET_TESTS() \
  RUN(test_set_init); \
  RUN(test_set_add_has); \
  RUN(test_set_values); \
  RUN(test_set_copies); \
  RUN(test_set_insert); \
  RUN(test_set_collisions); \
  RUN(test_set_delete); \
  RUN(test_set_delete_chain); \
  RUN(test_set_clear_free); \
  RUN(test_set_many); \
  RUN(test_set_first_last); \
  RUN(test_set_iterator); \
  RUN(test_set_combine); \
  RUN(test_set_text); \
  RUN(test_set_stralloc)

TESTS(set) { RUN_SET_TESTS(); }
