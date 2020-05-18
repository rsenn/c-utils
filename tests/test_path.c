#include "unit_test.h"
#include "../lib/buffer.h"
#include "../lib/path.h"
#include "../lib/stralloc.h"

/*
 * int path_is_absolute(const char*);
 */
TEST(test_path_is_absolute) {
  ASSERT_EQ(0, !path_is_absolute("/test/path"));
  ASSERT_EQ(1, !path_is_absolute("test/path"));
}

/*
 * char* path_basename(char*);
 */
TEST(test_path_basename) {
  ASSERT_EQ(0, str_diff("path", path_basename("/test/path")));
  ASSERT_EQ(0, str_diff("path", path_basename("test/path")));
  ASSERT_EQ(0, str_diff("path", path_basename("path")));
  ASSERT_EQ(0, !str_diff("path", path_basename("/path/test")));
}

/*
 * int path_canonicalize(const char*, stralloc* sa, int symbolic);
 */
TEST(test_path_canonicalize) {
  stralloc sa;
  stralloc_init(&sa);

  path_canonicalize("/test/path/test1/test2/../..", &sa, 0);

  buffer_puts(buffer_1, "\n\"");
  buffer_putsa(buffer_1, &sa);
  buffer_puts(buffer_1, "\"");
  buffer_putnlflush(buffer_1);

  ASSERT_EQ(0, stralloc_diffs(&sa, "/test/path"));
}

/*
 * char* path_dirname(char*, stralloc* dir);
 */
TEST(test_path_dirname) {
  stralloc sa;
  stralloc_init(&sa);

  ASSERT_EQ(0, str_diff("/test", path_dirname("/test/path", &sa)));
  ASSERT_EQ(0, str_diff("test", path_dirname("test/path", &sa)));
  ASSERT_EQ(0, str_diff(".", path_dirname("path", &sa)));
  ASSERT_EQ(0, !str_diff("test", path_dirname("/path/test", &sa)));
}

/*
 * int path_fnmatch(const char* pattern, unsigned int plen, const char* string, unsigned int slen, int flags);
 */

TEST(test_path_fnmatch) {}

/*
 * void path_getcwd(stralloc* sa);
 */

TEST(test_path_getcwd) {}

/*
 * char* path_gethome(int uid);
 */

TEST(test_path_gethome) {}

/*
 * size_t path_len_s(const char* s);
 */

TEST(test_path_len_s) {}

/*
 * size_t path_len(const char* s, size_t n);
 */

TEST(test_path_len) {}

/*
 * int path_readlink(const char* path, stralloc* sa);
 */

TEST(test_path_readlink) {}

/*
 * size_t path_right(const char* s, size_t n);
 */

TEST(test_path_right) {}

/*
 * size_t path_skips(const char* s);
 */

TEST(test_path_skips) {
  const char* p = "//blah/la/lumpur";
  size_t i = 0;
  ASSERT_EQ(2, (i += path_skips(&p[i])));
  ASSERT_EQ(7, (i += path_skips(&p[i])));
  ASSERT_EQ(10, (i += path_skips(&p[i])));
}

/*
 * size_t path_skip(const char* s, size_t n);
 */

TEST(test_path_skip) {}

/*
 * size_t path_num(const char* p, size_t len, int n);
 */

TEST(test_path_num) {}

/*
 * size_t path_num_sa(const char* p, size_t len, stralloc* sa, int n);
 */

TEST(test_path_num_sa) {}

/*
 * int path_exists(const char* p);
 */

TEST(test_path_exists) {}

/*
 * int path_relative(const char*, const char* to, stralloc* rel);
 */

TEST(test_path_relative) {}

/*
 * int path_split(const char* p, strlist* sl);
 */

TEST(test_path_split) {}

/*
 * int path_canonical_sa(stralloc*, stralloc* out);
 */

TEST(test_path_canonical_sa) {}

/*
 * int path_absolute_sa(stralloc*);
 */

TEST(test_path_absolute_sa) {}

/*
 * int path_collapse(const char*, stralloc* out);
 */

TEST(test_path_collapse) {

  stralloc sa;
  stralloc_init(&sa);

  path_collapse("/this/is/a/test/dir/..", 0);

  buffer_puts(buffer_1, "\n\"");
  buffer_putsa(buffer_1, &sa);
  buffer_puts(buffer_1, "\"");
  buffer_putnlflush(buffer_1);

  ASSERT_STRALLOCEQUALS(&sa, "/this/is/a/test");
}

#define RUN_PATH_TESTS()                                                                                               \
  RUN(test_path_basename);                                                                                             \
  RUN(test_path_canonicalize);                                                                                         \
  RUN(test_path_dirname);                                                                                              \
  RUN(test_path_fnmatch);                                                                                              \
  RUN(test_path_getcwd);                                                                                               \
  RUN(test_path_gethome);                                                                                              \
  RUN(test_path_len_s);                                                                                                \
  RUN(test_path_len);                                                                                                  \
  RUN(test_path_readlink);                                                                                             \
  RUN(test_path_right);                                                                                                \
  RUN(test_path_skips);                                                                                                \
  RUN(test_path_skip);                                                                                                 \
  RUN(test_path_num);                                                                                                  \
  RUN(test_path_num_sa);                                                                                               \
  RUN(test_path_exists);                                                                                               \
  RUN(test_path_is_absolute);                                                                                          \
  RUN(test_path_relative);                                                                                             \
  RUN(test_path_split);                                                                                                \
  RUN(test_path_canonical_sa);                                                                                         \
  RUN(test_path_absolute_sa);                                                                                          \
  RUN(test_path_collapse)

TESTS(path) { RUN_PATH_TESTS(); }
