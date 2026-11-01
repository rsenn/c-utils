#include "unit_test.h"
#include "../lib/critbit.h"
#include "../lib/str.h"

static int
count_cb(const char* s, void* arg) {
  (void)s;
  ++*(int*)arg;
  return 1;
}

static int
stop_cb(const char* s, void* arg) {
  (void)s;
  ++*(int*)arg;
  return 0;
}

/*
 * int critbit0_insert/contains/delete/clear/allprefixed (libowfat API)
 */
TEST(test_critbit_basic) {
  critbit0_tree t = {0};

  ASSERT_EQ(0, critbit0_contains(&t, "a"));
  ASSERT_EQ(2, critbit0_insert(&t, "a"));
  ASSERT_EQ(1, critbit0_contains(&t, "a"));
  ASSERT_EQ(1, critbit0_insert(&t, "a")); /* already there */
  ASSERT_EQ(2, critbit0_insert(&t, "ab"));
  ASSERT_EQ(2, critbit0_insert(&t, "b"));
  ASSERT_EQ(0, critbit0_contains(&t, "abc"));
  ASSERT_EQ(1, critbit0_delete(&t, "ab"));
  ASSERT_EQ(0, critbit0_contains(&t, "ab"));
  ASSERT_EQ(0, critbit0_delete(&t, "ab"));
  ASSERT_EQ(1, critbit0_contains(&t, "a"));
  critbit0_clear(&t);
  ASSERT_EQ(0, critbit0_contains(&t, "a"));
}

TEST(test_critbit_prefixed) {
  critbit0_tree t = {0};
  int n = 0;
  char key[16];
  int i;

  critbit0_insert(&t, "apple");
  critbit0_insert(&t, "apricot");
  critbit0_insert(&t, "banana");
  ASSERT_EQ(1, critbit0_allprefixed(&t, "ap", count_cb, &n));
  ASSERT_EQ(2, n);
  n = 0;
  ASSERT_EQ(1, critbit0_allprefixed(&t, "", count_cb, &n));
  ASSERT_EQ(3, n);
  n = 0;
  ASSERT_EQ(0, critbit0_allprefixed(&t, "ap", stop_cb, &n));
  ASSERT_EQ(1, n);
  n = 0;
  ASSERT_EQ(1, critbit0_allprefixed(&t, "zzz", count_cb, &n));
  ASSERT_EQ(0, n);
  critbit0_clear(&t);

  for(i = 0; i < 2000; ++i) {
    key[0] = 'k';
    key[1] = (char)('0' + i / 1000 % 10);
    key[2] = (char)('0' + i / 100 % 10);
    key[3] = (char)('0' + i / 10 % 10);
    key[4] = (char)('0' + i % 10);
    key[5] = 0;
    ASSERT_EQ(2, critbit0_insert(&t, key));
  }
  n = 0;
  critbit0_allprefixed(&t, "k0", count_cb, &n);
  ASSERT_EQ(1000, n);
  critbit0_clear(&t);
}

#define RUN_CRITBIT_TESTS() \
  RUN(test_critbit_basic); \
  RUN(test_critbit_prefixed)

TESTS(critbit) { RUN_CRITBIT_TESTS(); }
