#include "unit_test.h"
#include "../lib/strarray.h"
#include "../lib/stralloc.h"
#include "../lib/buffer.h"
#include "../lib/byte.h"
#include "../lib/str.h"
#include <stdlib.h>

/*
 * int strarray_from_argv(int argc, const char* const argv[], strarray* arr);
 */
TEST(test_strarray_from_argv) {
  const char* const headers[] = {"lib/array.h", "lib/buffer.h", "lib/xml.h"};
  strarray arr;

  strarray_init(&arr);
  strarray_from_argv(3, headers, &arr);

  ASSERT_EQ(3, strarray_size(&arr));
  ASSERT_STR_EQUAL("lib/array.h", strarray_at(&arr, 0));
  ASSERT_STR_EQUAL("lib/xml.h", strarray_at(&arr, 2));

  strarray_free(&arr);
}

/*
 * int strarray_removesuffixs(strarray* arr, const char* s);
 * int strarray_joins(const strarray* arr, stralloc* sa, const char* sep);
 */
TEST(test_strarray_removesuffixs_joins) {
  const char* const headers[] = {"lib/array.h", "lib/buffer.h", "lib/xml.h"};
  strarray arr;
  stralloc sa;

  strarray_init(&arr);
  stralloc_init(&sa);
  strarray_from_argv(3, headers, &arr);

  strarray_removesuffixs(&arr, ".h");
  strarray_joins(&arr, &sa, ", ");
  stralloc_nul(&sa);

  ASSERT_STR_EQUAL("lib/array, lib/buffer, lib/xml", sa.s);

  stralloc_free(&sa);
  strarray_free(&arr);
}

static ssize_t
discard(int fd, const void* x, size_t n, void* b) {
  (void)fd;
  (void)x;
  (void)b;
  return (ssize_t)n;
}

static char*
upcase(const char* s) {
  char* r = str_dup(s);
  char* p;

  for(p = r; *p; ++p)
    if(*p >= 'a' && *p <= 'z')
      *p -= 32;
  return r;
}

static void
hashfirst(char** p) {
  (*p)[0] = '#';
}

static int
cmp_str(const char** a, const char** b) {
  return str_diff(*a, *b);
}

/* true when arr holds exactly the n strings of v */
static int
holds(strarray* arr, size_t n, const char* const* v) {
  size_t i;

  if(strarray_size(arr) != n)
    return 0;
  for(i = 0; i < n; ++i)
    if(!strarray_at(arr, i) || str_diff(strarray_at(arr, i), v[i]))
      return 0;
  return 1;
}

/*
 * push, pushb, pushd, at, pop, set, setb, index_of*, push_sa
 */
TEST(test_strarray_push_pop_set) {
  strarray a;
  stralloc sa;
  char* p;

  strarray_init(&a);
  stralloc_init(&sa);
  ASSERT_EQ(1, strarray_push(&a, "one")); /* copies the string */
  strarray_push(&a, "two");
  ASSERT_EQ(1, strarray_pushb(&a, "threeXX", 5));
  ASSERT_EQ(1, strarray_pushd(&a, "four"));
  ASSERT_EQ(4, strarray_size(&a));
  ASSERT_STR_EQUAL("three", strarray_at(&a, 2));
  ASSERT_EQ(1, strarray_at(&a, 9) == NULL);

  ASSERT_EQ(1, strarray_index_of(&a, "two"));
  ASSERT_EQ(-1, strarray_index_of(&a, "zzz"));
  ASSERT_EQ(2, strarray_index_of_b(&a, "three", 5));
  ASSERT_EQ(-1, strarray_index_of_b(&a, "thr", 3)); /* whole string compare */
  stralloc_copys(&sa, "four");
  ASSERT_EQ(3, strarray_index_of_sa(&a, &sa));

  p = strarray_pop(&a); /* the caller owns the popped string */
  ASSERT_STR_EQUAL("four", p);
  free(p);
  ASSERT_EQ(3, strarray_size(&a));

  ASSERT_EQ(1, strarray_set(&a, 1, "TWO"));
  ASSERT_EQ(1, strarray_setb(&a, 0, "onexx", 3));
  ASSERT_STR_EQUAL("one", strarray_at(&a, 0));
  ASSERT_STR_EQUAL("TWO", strarray_at(&a, 1));

  ASSERT_EQ(1, strarray_push_sa(&a, &sa));
  ASSERT_STR_EQUAL("four", strarray_at(&a, 3));

  strarray_free(&a);
  stralloc_free(&sa);
}

TEST(test_strarray_unique) {
  strarray a;

  strarray_init(&a);
  strarray_push(&a, "x");
  ASSERT_EQ(0, strarray_push_unique(&a, "x")); /* already there */
  ASSERT_EQ(1, strarray_push_unique(&a, "y"));
  ASSERT_EQ(0, strarray_pushb_unique(&a, "yyyy", 1));
  ASSERT_EQ(1, strarray_pushb_unique(&a, "zzzz", 2));
  ASSERT_EQ(3, strarray_size(&a));
  ASSERT_STR_EQUAL("zz", strarray_at(&a, 2));
  strarray_free(&a);
}

/*
 * unshift, unshiftb, unshiftm, unshiftn put COPIES at the front: the array can
 * be freed afterwards (they used to store the caller's pointers)
 */
TEST(test_strarray_unshift) {
  strarray a;
  static const char* const expect[] = {"n1", "n2", "m1", "m2", "zero", "first", "one"};
  const char* v[] = {"n1", "n2"};

  strarray_init(&a);
  strarray_push(&a, "one");
  ASSERT_EQ(1, strarray_unshift(&a, "first"));
  ASSERT_EQ(1, strarray_unshiftb(&a, "zeroXX", 4));
  ASSERT_EQ(0, strarray_unshiftm(&a, "m1", "m2", (char*)0));
  ASSERT_EQ(1, strarray_unshiftn(&a, v, 2));
  ASSERT_EQ(1, holds(&a, 7, expect));
  strarray_free(&a); /* frees only what the array owns */
}

TEST(test_strarray_edit_all) {
  strarray a, out;
  static const char* const with_prefix[] = {"lib/a.h!", "lib/b.h!"};
  static const char* const plain[] = {"a.h!", "b.h!"};

  strarray_init(&a);
  strarray_init(&out);
  strarray_push(&a, "a.h");
  strarray_push(&a, "b.h");
  ASSERT_EQ(1, strarray_appends(&a, "!"));
  ASSERT_EQ(1, strarray_prepends(&a, "lib/"));
  ASSERT_EQ(1, holds(&a, 2, with_prefix));
  ASSERT_EQ(1, strarray_removeprefixs(&a, "lib/"));
  ASSERT_EQ(1, holds(&a, 2, plain));

  ASSERT_EQ(2, strarray_transform(&a, &out, upcase));
  ASSERT_STR_EQUAL("A.H!", strarray_at(&out, 0));
  ASSERT_STR_EQUAL("B.H!", strarray_at(&out, 1));

  strarray_each(&a, hashfirst);
  ASSERT_STR_EQUAL("#.h!", strarray_at(&a, 0));

  strarray_free(&a);
  strarray_free(&out);
}

TEST(test_strarray_sort) {
  strarray a;
  static const char* const sorted[] = {"apple", "fig", "pear"};

  strarray_init(&a);
  strarray_push(&a, "pear");
  strarray_push(&a, "apple");
  strarray_push(&a, "fig");
  strarray_sort(&a, cmp_str);
  ASSERT_EQ(1, holds(&a, 3, sorted));
  strarray_free(&a);
}

TEST(test_strarray_splice) {
  strarray a;
  static const char* const r1[] = {"A", "x", "y", "D"};
  static const char* const r2[] = {"A", "x", "y", "D", "z"};
  static const char* const r3[] = {"A", "D", "z"};
  static const char* ins[] = {"x", "y"};
  const char* one[] = {"z"};

  strarray_init(&a);
  strarray_push(&a, "A");
  strarray_push(&a, "B");
  strarray_push(&a, "C");
  strarray_push(&a, "D");
  ASSERT_EQ(4, strarray_splice(&a, 1, 2, 2, ins)); /* replace B C by x y */
  ASSERT_EQ(1, holds(&a, 4, r1));
  ASSERT_EQ(5, strarray_splice(&a, 99, 5, 1, one)); /* start past the end appends */
  ASSERT_EQ(1, holds(&a, 5, r2));
  ASSERT_EQ(3, strarray_splice(&a, 1, 2, 0, NULL)); /* delete only */
  ASSERT_EQ(1, holds(&a, 3, r3));
  ASSERT_EQ(3, strarray_splice(&a, 1, 0, 0, NULL)); /* nothing */
  strarray_free(&a);

  strarray_init(&a); /* splicing into an empty array */
  ASSERT_EQ(2, strarray_splice(&a, 0, 0, 2, ins));
  strarray_free(&a);
}

TEST(test_strarray_argv_vec_copy) {
  strarray a, b, c;
  char* vec[] = {"v1", "v2", 0};
  char** argv;
  static const char* const v12[] = {"v1", "v2"};

  strarray_init(&b);
  strarray_init(&c);
  ASSERT_EQ(2, strarray_from_vec(&a, vec));
  ASSERT_EQ(1, holds(&a, 2, v12));

  argv = strarray_to_argv(&a);
  ASSERT_STR_EQUAL("v1", argv[0]);
  ASSERT_EQ(1, argv[2] == NULL); /* NULL terminated vector */

  ASSERT_EQ(1, strarray_copy(&b, &a));
  ASSERT_EQ(1, holds(&b, 2, v12));
  ASSERT_EQ(1, b.p[0] != a.p[0]); /* a deep copy */

  strarray_free(&a);
  strarray_free(&b);
  strarray_free(&c);
}

TEST(test_strarray_set_operations) {
  strarray a, b, u, i;
  static const char* const exp_u[] = {"a", "b", "c", "d"};
  static const char* const exp_i[] = {"b", "c"};

  strarray_init(&a);
  strarray_init(&b);
  strarray_init(&u);
  strarray_init(&i);
  strarray_push(&a, "a");
  strarray_push(&a, "b");
  strarray_push(&a, "c");
  strarray_push(&b, "c"); /* neither has to be sorted */
  strarray_push(&b, "d");
  strarray_push(&b, "b");
  strarray_union(&a, &b, &u);
  ASSERT_EQ(1, holds(&u, 4, exp_u));
  strarray_intersection(&a, &b, &i);
  ASSERT_EQ(1, holds(&i, 2, exp_i));

  strarray_free(&u);
  strarray_init(&u);
  strarray_union(&a, &u, &i); /* an empty operand used to crash */
  strarray_free(&a);
  strarray_free(&b);
  strarray_free(&u);
  strarray_free(&i);
}

TEST(test_strarray_glob) {
  strarray a;

  strarray_init(&a);
  ASSERT_EQ(1, strarray_glob(&a, "/etc/host*") >= 1); /* number of matches */
  ASSERT_EQ(1, strarray_size(&a) >= 1);
  ASSERT_EQ(0, strarray_glob_b(&a, "/nonexistent-dir/*", 18)); /* no match is not an error */
  strarray_free(&a);
}

TEST(test_strarray_output) {
  strarray a;
  buffer b;
  char mem[256], got[64];
  stralloc sa;

  strarray_init(&a);
  stralloc_init(&sa);
  strarray_push(&a, "A");
  strarray_push(&a, "B");
  buffer_init(&b, (buffer_op_ptr)discard, -1, mem, sizeof mem);
  buffer_putstra(&b, &a, ", ");
  ASSERT_EQ(4, b.p);
  ASSERT_EQ(0, byte_diff(mem, 4, "A, B"));
  b.p = 0;
  strarray_dump(&b, &a); /* ends with a flush, which the discarding writer swallows */
  ASSERT_EQ(0, b.p);
  (void)got;
  strarray_free(&a);
  stralloc_free(&sa);
}

#define RUN_STRARRAY_TESTS() \
  RUN(test_strarray_push_pop_set); \
  RUN(test_strarray_unique); \
  RUN(test_strarray_unshift); \
  RUN(test_strarray_edit_all); \
  RUN(test_strarray_sort); \
  RUN(test_strarray_splice); \
  RUN(test_strarray_argv_vec_copy); \
  RUN(test_strarray_set_operations); \
  RUN(test_strarray_glob); \
  RUN(test_strarray_output); \
  RUN(test_strarray_from_argv); \
  RUN(test_strarray_removesuffixs_joins)

TESTS(strarray) { RUN_STRARRAY_TESTS(); }
