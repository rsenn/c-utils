#include "unit_test.h"
#include "../lib/stralloc.h"
#include "../lib/env.h"
#include "../lib/str.h"
#include "../lib/unix.h"

/*
 * int env_unset(const char* name);
 *
 * env_unset() now routes through the same en/ea-bookkept
 * env_unset_internal() (lib/env/env_put.c) that env_put()/env_set() use,
 * instead of shelling out to plain unsetenv() behind env_init()'s back
 * (see BUGS: env-unset-desyncs-env-init-bookkeeping, now fixed). The
 * env_set() call after env_unset() below exercises exactly the sequence
 * that used to desync the bookkeeping and corrupt `environ`'s NUL
 * terminator -- it must still find a correctly terminated array to grow.
 */
TEST(test_env_unset) {
  ASSERT_NE(0, env_init());
  ASSERT_NE(0, env_set("CUTILS_TEST_UNSET", "x"));
  ASSERT_NE(0, env_unset("CUTILS_TEST_UNSET"));
  ASSERT_EQ(NULL, env_get("CUTILS_TEST_UNSET"));

  ASSERT_NE(0, env_set("CUTILS_TEST_AFTER_UNSET", "y"));
  ASSERT_EQ(0, str_diff(env_get("CUTILS_TEST_AFTER_UNSET"), "y"));
}

/*
 * int env_set(const char* name, const char* value);
 * const char* env_get(const char* s);
 */
TEST(test_env_set_get) {
  ASSERT_NE(0, env_set("CUTILS_TEST_VAR", "value1"));
  ASSERT_EQ(0, str_diff(env_get("CUTILS_TEST_VAR"), "value1"));
}

/*
 * const char* env_get2(const char* const* envp, const char* s);
 */
TEST(test_env_get2) {
  static const char* const envp[] = {"FOO=bar", "BAZ=qux", NULL};

  ASSERT_EQ(0, str_diff(env_get2(envp, "FOO"), "bar"));
  ASSERT_EQ(0, str_diff(env_get2(envp, "BAZ"), "qux"));
  ASSERT_EQ(NULL, env_get2(envp, "MISSING"));
}

/*
 * const char* env_get_b(const char* s, size_t namelen);
 */
TEST(test_env_get_b) {
  env_set("CUTILS_TEST_B", "hello");
  ASSERT_EQ(0, str_diff(env_get_b("CUTILS_TEST_B", 13), "hello"));
}

/*
 * size_t env_len(const char* const* e);
 */
TEST(test_env_len) {
  static const char* const envp[] = {"A=1", "B=2", "C=3", NULL};

  ASSERT_EQ(3, env_len(envp));
}

/*
 * int env_put(const char* s);
 * int env_put2(const char* s, const char* t);
 * int env_put2b(const char* s, const char* t, size_t n);
 * int env_putb(const char*, size_t);
 */
TEST(test_env_put_variants) {
  ASSERT_NE(0, env_put("CUTILS_TEST_PUT=one"));
  ASSERT_EQ(0, str_diff(env_get("CUTILS_TEST_PUT"), "one"));

  ASSERT_NE(0, env_put2("CUTILS_TEST_PUT2", "two"));
  ASSERT_EQ(0, str_diff(env_get("CUTILS_TEST_PUT2"), "two"));

  ASSERT_NE(0, env_put2b("CUTILS_TEST_PUT3", "threeXXX", 5));
  ASSERT_EQ(0, str_diff(env_get("CUTILS_TEST_PUT3"), "three"));

  ASSERT_NE(0, env_putb("CUTILS_TEST_PUT4=fourXXX", 21));
  ASSERT_EQ(0, str_diff(env_get("CUTILS_TEST_PUT4"), "four"));
}

/*
 * size_t env_init(void);
 */
TEST(test_env_init) { ASSERT_NE(0, env_init()); }

/*
 * int env_make(const char** v, size_t argc, const char* s, size_t len);
 */
TEST(test_env_make) {
  const char* v[2];

  ASSERT_NE(0, env_make(v, 2, "A=1\0B=2\0", 8));
  ASSERT_EQ(0, str_diff(v[0], "A=1"));
  ASSERT_EQ(0, str_diff(v[1], "B=2"));
}

/*
 * size_t env_merge(const char** v, size_t vmax, const char* const* envp, size_t envlen, const char* modifs, size_t
 * modiflen);
 */
TEST(test_env_merge) {
  static const char* const envp[] = {"A=1", "B=2"};
  const char* v[8];
  size_t n;

  n = env_merge(v, 8, envp, 2, "B=3\0C=4\0", 8);

  ASSERT_GE(n, 2);
}

/*
 * char* env_pick(void);
 */
TEST(test_env_pick) {
  char* p = env_pick();

  (void)p;
}

/*
 * void env_clear(void);
 *
 * Not exercised directly: it would clobber the process's actual
 * environment for the remainder of the test run.
 */
TEST(test_env_clear) {}

/*
 * int env_addmodif(stralloc* sa, const char* s, const char* t);
 * int env_string(stralloc* sa, const char* const* envp, size_t envlen);
 */
TEST(test_env_addmodif_string) {
  stralloc sa;
  static const char* const envp[] = {"A=1", "B=2"};

  stralloc_init(&sa);

  ASSERT_NE(0, env_addmodif(&sa, "A", "9"));
  ASSERT_NE(0, env_string(&sa, envp, 2));

  stralloc_free(&sa);
}

#define RUN_ENV_TESTS() \
  RUN(test_env_unset); \
  RUN(test_env_set_get); \
  RUN(test_env_get2); \
  RUN(test_env_get_b); \
  RUN(test_env_len); \
  RUN(test_env_put_variants); \
  RUN(test_env_init); \
  RUN(test_env_make); \
  RUN(test_env_merge); \
  RUN(test_env_pick); \
  RUN(test_env_clear); \
  RUN(test_env_addmodif_string)

TESTS(env) { RUN_ENV_TESTS(); }
