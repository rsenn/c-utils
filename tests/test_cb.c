#include "unit_test.h"
#include "../lib/cb.h"
#include "../lib/str.h"
#include "../lib/byte.h"
#include <stdio.h>

static int visited;
static char last_match[32];

static int
count_cb(const void* match, const void* key, size_t keylen, void* data) {
  (void)key;
  (void)keylen;
  (void)data;
  visited++;
  str_copy(last_match, (const char*)match);
  return 0;
}

static int
stop_at_second(const void* match, const void* key, size_t keylen, void* data) {
  (void)match;
  (void)key;
  (void)keylen;
  (void)data;
  return ++visited == 2;
}

/*
 * int cb_insert(critbit_tree*, const void*, size_t);
 * const void* cb_find(critbit_tree*, const void*, size_t);
 * cb_insert_str / cb_find_str
 */
TEST(test_cb_insert_find) {
  critbit_tree cb = CRITBIT_TREE();
  const char* p;

  ASSERT_EQ(NULL, cb_find_str(&cb, "alpha"));

  ASSERT_EQ(CB_SUCCESS, cb_insert_str(&cb, "alpha"));
  ASSERT_EQ(CB_SUCCESS, cb_insert_str(&cb, "beta"));
  ASSERT_EQ(CB_SUCCESS, cb_insert_str(&cb, "alphabet"));
  ASSERT_EQ(CB_SUCCESS, cb_insert_str(&cb, ""));

  /* inserting twice says so, and does not add */
  ASSERT_EQ(CB_EXISTS, cb_insert_str(&cb, "beta"));

  p = cb_find_str(&cb, "alpha");
  ASSERT_NE(NULL, p);
  ASSERT_STR_EQUAL("alpha", p);
  ASSERT_STR_EQUAL("alphabet", cb_find_str(&cb, "alphabet"));
  ASSERT_STR_EQUAL("beta", cb_find_str(&cb, "beta"));
  ASSERT_STR_EQUAL("", cb_find_str(&cb, ""));

  ASSERT_EQ(NULL, cb_find_str(&cb, "gamma"));
  ASSERT_EQ(NULL, cb_find_str(&cb, "alph"));
  ASSERT_EQ(NULL, cb_find_str(&cb, "alphabets"));

  cb_clear(&cb);
}

/* keys are bytes, not only strings */
TEST(test_cb_binary_keys) {
  critbit_tree cb = CRITBIT_TREE();
  static const unsigned char a[3] = {1, 0, 2}, b[3] = {1, 0, 3}, c[2] = {1, 0};

  ASSERT_EQ(CB_SUCCESS, cb_insert(&cb, a, 3));
  ASSERT_EQ(CB_SUCCESS, cb_insert(&cb, b, 3));
  ASSERT_EQ(CB_SUCCESS, cb_insert(&cb, c, 2));
  ASSERT_EQ(CB_EXISTS, cb_insert(&cb, a, 3));

  ASSERT_NE(NULL, cb_find(&cb, a, 3));
  ASSERT_NE(NULL, cb_find(&cb, b, 3));
  ASSERT_NE(NULL, cb_find(&cb, c, 2));
  ASSERT_EQ(0, byte_diff(cb_find(&cb, a, 3), 3, a));
  ASSERT_EQ(NULL, cb_find(&cb, "\x01\x01", 2));

  cb_clear(&cb);
}

/*
 * int cb_erase(critbit_tree*, const void*, size_t);
 */
TEST(test_cb_erase) {
  critbit_tree cb = CRITBIT_TREE();

  ASSERT_EQ(CB_NOTFOUND, cb_erase_str(&cb, "x"));

  cb_insert_str(&cb, "one");
  cb_insert_str(&cb, "two");
  cb_insert_str(&cb, "three");

  ASSERT_EQ(CB_SUCCESS, cb_erase_str(&cb, "two"));
  ASSERT_EQ(NULL, cb_find_str(&cb, "two"));
  ASSERT_NE(NULL, cb_find_str(&cb, "one"));
  ASSERT_NE(NULL, cb_find_str(&cb, "three"));

  ASSERT_EQ(CB_NOTFOUND, cb_erase_str(&cb, "two"));
  ASSERT_EQ(CB_NOTFOUND, cb_erase_str(&cb, "thr"));

  ASSERT_EQ(CB_SUCCESS, cb_erase_str(&cb, "one"));
  ASSERT_EQ(CB_SUCCESS, cb_erase_str(&cb, "three"));
  ASSERT_EQ(NULL, cb.root);

  /* it works again */
  ASSERT_EQ(CB_SUCCESS, cb_insert_str(&cb, "one"));
  ASSERT_NE(NULL, cb_find_str(&cb, "one"));
  cb_clear(&cb);
}

/*
 * int cb_find_prefix(const critbit_tree*, const void*, size_t, void**, int, int);
 */
TEST(test_cb_find_prefix) {
  critbit_tree cb = CRITBIT_TREE();
  void* res[8];
  int n;

  cb_insert_str(&cb, "car");
  cb_insert_str(&cb, "card");
  cb_insert_str(&cb, "care");
  cb_insert_str(&cb, "cat");
  cb_insert_str(&cb, "dog");

  /* in the order of the keys */
  n = cb_find_prefix_str(&cb, "car", res, 8, 0);
  ASSERT_EQ(3, n);
  ASSERT_STR_EQUAL("car", res[0]);
  ASSERT_STR_EQUAL("card", res[1]);
  ASSERT_STR_EQUAL("care", res[2]);

  n = cb_find_prefix_str(&cb, "c", res, 8, 0);
  ASSERT_EQ(4, n);
  ASSERT_STR_EQUAL("cat", res[3]);

  /* the size of the result and the offset */
  ASSERT_EQ(2, cb_find_prefix_str(&cb, "c", res, 2, 0));
  ASSERT_STR_EQUAL("car", res[0]);
  ASSERT_STR_EQUAL("card", res[1]);
  ASSERT_EQ(2, cb_find_prefix_str(&cb, "c", res, 2, 2));
  ASSERT_STR_EQUAL("care", res[0]);
  ASSERT_STR_EQUAL("cat", res[1]);

  ASSERT_EQ(0, cb_find_prefix_str(&cb, "x", res, 8, 0));
  ASSERT_EQ(0, cb_find_prefix_str(&cb, "c", res, 0, 0));

  /* the empty prefix is everything */
  ASSERT_EQ(5, cb_find_prefix(&cb, "", 0, res, 8, 0));

  cb_clear(&cb);
}

/*
 * int cb_foreach(critbit_tree*, const void*, size_t, callback, void*);
 */
TEST(test_cb_foreach) {
  critbit_tree cb = CRITBIT_TREE();

  cb_insert_str(&cb, "car");
  cb_insert_str(&cb, "card");
  cb_insert_str(&cb, "cat");
  cb_insert_str(&cb, "dog");

  visited = 0;
  ASSERT_EQ(0, cb_foreach(&cb, "ca", 2, count_cb, NULL));
  ASSERT_EQ(3, visited);
  ASSERT_STR_EQUAL("cat", last_match);

  visited = 0;
  ASSERT_EQ(0, cb_foreach(&cb, "x", 1, count_cb, NULL));
  ASSERT_EQ(0, visited);

  /* the walk ends when the callback does not return 0, and passes the value on */
  visited = 0;
  ASSERT_NE(0, cb_foreach(&cb, "", 0, stop_at_second, NULL));
  ASSERT_EQ(2, visited);

  cb_clear(&cb);
}

/*
 * void cb_clear(critbit_tree*);
 */
TEST(test_cb_clear) {
  critbit_tree cb = CRITBIT_TREE();
  int i;
  char key[16];

  for(i = 0; i < 200; i++) {
    snprintf(key, sizeof(key), "key%d", i);
    ASSERT_EQ(CB_SUCCESS, cb_insert_str(&cb, key));
  }
  cb_clear(&cb);
  ASSERT_EQ(NULL, cb.root);
  ASSERT_EQ(NULL, cb_find_str(&cb, "key5"));
  cb_clear(&cb);
}

/* many keys, in the order they were not inserted */
TEST(test_cb_many) {
  critbit_tree cb = CRITBIT_TREE();
  char key[16];
  void* res[512];
  int i;

  for(i = 0; i < 500; i++) {
    snprintf(key, sizeof(key), "k%03d", (i * 37) % 500);
    ASSERT_EQ(CB_SUCCESS, cb_insert_str(&cb, key));
  }
  for(i = 0; i < 500; i++) {
    snprintf(key, sizeof(key), "k%03d", i);
    ASSERT_NE(NULL, cb_find_str(&cb, key));
  }

  ASSERT_EQ(500, cb_find_prefix(&cb, "k", 1, res, 512, 0));
  for(i = 1; i < 500; i++)
    ASSERT_LT(str_diff(res[i - 1], res[i]), 0);

  for(i = 0; i < 500; i += 2) {
    snprintf(key, sizeof(key), "k%03d", i);
    ASSERT_EQ(CB_SUCCESS, cb_erase_str(&cb, key));
  }
  ASSERT_EQ(250, cb_find_prefix(&cb, "k", 1, res, 512, 0));
  ASSERT_EQ(NULL, cb_find_str(&cb, "k000"));
  ASSERT_NE(NULL, cb_find_str(&cb, "k001"));

  cb_clear(&cb);
}

/*
 * size_t cb_new_kv(const char*, size_t, const void*, size_t, void*);
 * void cb_get_kv(const void*, void*, size_t);
 * void cb_get_kv_ex(void*, void**);
 */
TEST(test_cb_kv) {
  char buf[64];
  int v = 0x1234, out = 0;
  void* p;
  size_t n;

  n = cb_new_kv("key", 3, &v, sizeof(v), buf);
  ASSERT_EQ(3 + 1 + sizeof(v), n);
  ASSERT_EQ(CB_KV_SIZE(3, sizeof(v)), n);
  ASSERT_STR_EQUAL("key", buf);

  cb_get_kv(buf, &out, sizeof(out));
  ASSERT_EQ(0x1234, out);

  cb_get_kv_ex(buf, &p);
  ASSERT_EQ((void*)(buf + 4), p);
  ASSERT_EQ(0x1234, *(int*)p);
}

/* a pair as a key: the tree finds it by the key part */
TEST(test_cb_kv_in_tree) {
  critbit_tree cb = CRITBIT_TREE();
  char buf[64];
  int v = 7, out = 0;
  const char* kv;

  cb_new_kv("answer", 6, &v, sizeof(v), buf);
  ASSERT_EQ(CB_SUCCESS, cb_insert(&cb, buf, CB_KV_SIZE(6, sizeof(v))));

  kv = cb_find(&cb, "answer", 7);
  ASSERT_NE(NULL, kv);
  cb_get_kv(kv, &out, sizeof(out));
  ASSERT_EQ(7, out);

  cb_clear(&cb);
}

#define RUN_CB_TESTS() \
  RUN(test_cb_insert_find); \
  RUN(test_cb_binary_keys); \
  RUN(test_cb_erase); \
  RUN(test_cb_find_prefix); \
  RUN(test_cb_foreach); \
  RUN(test_cb_clear); \
  RUN(test_cb_many); \
  RUN(test_cb_kv); \
  RUN(test_cb_kv_in_tree)

TESTS(cb) { RUN_CB_TESTS(); }
