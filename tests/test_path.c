#include "unit_test.h"
#include "../lib/buffer.h"
#include "../lib/byte.h"
#include "../lib/path.h"
#include "../lib/stralloc.h"

/*
 * int path_is_absolute(const char*);
 */
TEST(test_path_is_absolute) {
  ASSERT_EQ(0, !path_is_absolute_s("/test/path"));
  ASSERT_EQ(1, !path_is_absolute_s("test/path"));
}

/*
 * const char* path_basename(const char* p, size_t n, size_t* len);
 */
TEST(test_path_basename) {
  size_t len;
  const char* b;

  ASSERT_EQ(0, str_diff("path", path_basename_s("/test/path")));
  ASSERT_EQ(0, str_diff("path", path_basename_s("test/path")));
  ASSERT_EQ(0, str_diff("path", path_basename_s("path")));

  b = path_basename("/usr/lib//", 10, &len);
  ASSERT_EQ(3, len);
  ASSERT_STR_EQUAL_N(b, "lib", 3);

  b = path_basename("/", 1, &len);
  ASSERT_EQ(1, len);
  b = path_basename("", 0, &len);
  ASSERT_EQ(0, len);
}

/*
 * size_t path_rootlen(const char* p, size_t n);
 */
TEST(test_path_rootlen) {
  ASSERT_EQ(1, path_rootlen("/x", 2));
  ASSERT_EQ(1, path_rootlen("//x", 3));
  ASSERT_EQ(0, path_rootlen("x", 1));
  ASSERT_EQ(0, path_rootlen("", 0));
}

/*
 * size_t path_cat(char* out, size_t cap, const char* a, size_t an, const char* b, size_t bn);
 */
TEST(test_path_cat) {
  char buf[16];
  size_t n;

  n = path_cat(buf, sizeof buf, "a", 1, "b", 1);
  ASSERT_EQ(3, n);
  ASSERT_STR_EQUAL_N(buf, "a/b", 3);
  n = path_cat(buf, sizeof buf, "a/", 2, "b", 1);
  ASSERT_STR_EQUAL_N(buf, "a/b", n);
  n = path_cat(buf, sizeof buf, "", 0, "b", 1);
  ASSERT_STR_EQUAL_N(buf, "b", n);
  n = path_cat(buf, sizeof buf, "a", 1, "", 0);
  ASSERT_STR_EQUAL_N(buf, "a", n);
  n = path_cat(buf, sizeof buf, "a", 1, "/b", 2);
  ASSERT_STR_EQUAL_N(buf, "/b", n);

  /* in place: the walker idiom */
  buf[0] = 'a';
  n = path_cat(buf, sizeof buf, buf, 1, "bc", 2);
  ASSERT_EQ(4, n);
  ASSERT_STR_EQUAL_N(buf, "a/bc", 4);

  /* all or nothing: too small a buffer leaves out untouched and returns the needed length */
  byte_copy(buf, 4, "xxxx");
  n = path_cat(buf, 4, "a", 1, "bc", 2);
  ASSERT_EQ(4, n);
  ASSERT_STR_EQUAL_N(buf, "xxxx", 4);
  ASSERT_EQ(4, path_cat(0, 0, "a", 1, "bc", 2));
}

/*
 * size_t path_canon(char* out, size_t cap, const char* p, size_t n);
 */
TEST(test_path_canon) {
  static const char* const tab[][2] = {
      {"", "."},
      {".", "."},
      {"..", ".."},
      {"/", "/"},
      {"//", "/"},
      {"/..", "/"},
      {"a/", "a"},
      {"a//b/./c/../d", "a/b/d"},
      {"a/b/..", "a"},
      {"a/..", "."},
      {"../..", "../.."},
      {"x/../../y", "../y"},
      {"./a", "a"},
      {"/a/b/../../..", "/"},
      {"///a///b///", "/a/b"},
      {"/this/is/a/test/dir/..", "/this/is/a/test"},
  };
  char buf[64];
  size_t i, n;

  for(i = 0; i < sizeof(tab) / sizeof(tab[0]); ++i) {
    n = path_canon(buf, sizeof buf, tab[i][0], str_len(tab[i][0]));
    ASSERT_EQ(str_len(tab[i][1]), n);
    ASSERT_STR_EQUAL_N(buf, tab[i][1], n);
    ASSERT_EQ(0, buf[n]);
  }

  /* in place */
  byte_copy(buf, 8, "a/b/../c");
  n = path_canon(buf, sizeof buf, buf, 8);
  ASSERT_EQ(3, n);
  ASSERT_STR_EQUAL_N(buf, "a/c", 3);

  /* all or nothing: needs cap >= n + 2, otherwise untouched and n + 1 */
  byte_copy(buf, 4, "xxxx");
  ASSERT_EQ(4, path_canon(buf, 4, "a/b", 3));
  ASSERT_STR_EQUAL_N(buf, "xxxx", 4);
}

/*
 * size_t path_dirlen(const char* p, size_t n);
 */
TEST(test_path_dirlen) {
  ASSERT_EQ(4, path_dirlen("/usr/lib/", 9));
  ASSERT_EQ(1, path_dirlen("/usr", 4));
  ASSERT_EQ(1, path_dirlen("a//b", 4));
  ASSERT_EQ(0, path_dirlen("usr", 3));
  ASSERT_EQ(1, path_dirlen("/", 1));
  ASSERT_EQ(0, path_dirlen("", 0));
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
 * char* path_dirname_s(char*, stralloc* dir);
 */
TEST(test_path_dirname) {
  stralloc sa;
  stralloc_init(&sa);

  ASSERT_EQ(0, str_diff("/test", path_dirname_s("/test/path", &sa)));
  ASSERT_EQ(0, str_diff("test", path_dirname_s("test/path", &sa)));
  ASSERT_EQ(0, str_diff(".", path_dirname_s("path", &sa)));
  ASSERT_EQ(0, !str_diff("test", path_dirname_s("/path/test", &sa)));
  ASSERT_EQ(0, str_diff("/", path_dirname_s("/usr", &sa)));
  ASSERT_EQ(0, str_diff("/", path_dirname_s("/", &sa)));
  ASSERT_EQ(0, str_diff(".", path_dirname_s("usr/", &sa)));
  ASSERT_EQ(0, str_diff(".", path_dirname_s(".", &sa)));
  ASSERT_EQ(0, str_diff("/usr", path_dirname_s("/usr/lib/", &sa)));
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
 * size_t path_dirlen(const char* s, size_t n);
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
 * int path_relative_to_s(const char*, const char* to, stralloc* rel);
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
  char buf[64] = "/this/is/a/test/dir/..";
  size_t n = path_canon(buf, sizeof buf, buf, str_len(buf));

  ASSERT_EQ(str_len("/this/is/a/test"), n);
  ASSERT_STR_EQUAL_N(buf, "/this/is/a/test", n);
}

/*
 * size_t path_rel(char* out, size_t cap, const char* p, size_t pn, const char* base, size_t bn);
 */
TEST(test_path_rel) {
  static const char* const tab[][3] = {
      {"/a/b/c", "/a/d", "../b/c"},
      {"/a", "/a", "."},
      {"a", "a/b/c", "../.."},
      {"/a/b", "/a", "b"},
      {"/usr/local/bin", "/usr/local/bin/x/y", "../.."},
      {"a/b", "c/d", "../../a/b"},
  };
  char buf[64];
  size_t i, n;

  for(i = 0; i < sizeof(tab) / sizeof(tab[0]); ++i) {
    n = path_rel(buf, sizeof buf, tab[i][0], str_len(tab[i][0]), tab[i][1], str_len(tab[i][1]));
    ASSERT_EQ(str_len(tab[i][2]), n);
    ASSERT_STR_EQUAL_N(buf, tab[i][2], n);
  }

  ASSERT_EQ((size_t)-1, path_rel(buf, sizeof buf, "/a", 2, "b", 1));
  byte_copy(buf, 4, "xxxx");
  ASSERT_EQ(6, path_rel(buf, 4, "/a/b/c", 6, "/a/d", 4));
  ASSERT_STR_EQUAL_N(buf, "xxxx", 4);
}

/*
 * size_t path_abs(char* out, size_t cap, const char* p, size_t n, const char* cwd, size_t cwdn);
 */
TEST(test_path_abs) {
  char buf[64];
  size_t n;

  n = path_abs(buf, sizeof buf, "x/../y", 6, "/a/b", 4);
  ASSERT_STR_EQUAL_N(buf, "/a/b/y", n);
  n = path_abs(buf, sizeof buf, "/z", 2, "/a/b", 4);
  ASSERT_STR_EQUAL_N(buf, "/z", n);
  n = path_abs(buf, sizeof buf, ".", 1, "/a/b", 4);
  ASSERT_STR_EQUAL_N(buf, "/a/b", n);
}

/*
 * size_t path_common(const char* a, size_t an, const char* b, size_t bn, size_t* blen);
 */
TEST(test_path_common) {
  size_t bl;

  ASSERT_EQ(10, path_common("/usr/local/bin", 14, "//usr//local//share", 19, &bl));
  ASSERT_EQ(12, bl);
  ASSERT_EQ(0, path_common("a/b", 3, "c/b", 3, &bl));
  ASSERT_EQ(0, path_common("/a", 2, "a", 1, &bl));
}

/*
 * size_t path_extoff(const char* p, size_t n);
 */
TEST(test_path_extoff) {
  ASSERT_EQ(7, path_extoff("a/b.tar.gz", 10));
  ASSERT_EQ(7, path_extoff(".bashrc", 7));
  ASSERT_EQ(5, path_extoff("a.d/b", 5));
  ASSERT_EQ(1, path_extoff("a.", 2));
  ASSERT_EQ(2, path_extoff("..", 2));
}

#define RUN_PATH_TESTS() \
  RUN(test_path_basename); \
  RUN(test_path_rootlen); \
  RUN(test_path_cat); \
  RUN(test_path_canon); \
  RUN(test_path_dirlen); \
  RUN(test_path_rel); \
  RUN(test_path_abs); \
  RUN(test_path_common); \
  RUN(test_path_extoff); \
  RUN(test_path_canonicalize); \
  RUN(test_path_dirname); \
  RUN(test_path_fnmatch); \
  RUN(test_path_getcwd); \
  RUN(test_path_gethome); \
  RUN(test_path_len_s); \
  RUN(test_path_len); \
  RUN(test_path_readlink); \
  RUN(test_path_right); \
  RUN(test_path_skips); \
  RUN(test_path_skip); \
  RUN(test_path_num); \
  RUN(test_path_num_sa); \
  RUN(test_path_exists); \
  RUN(test_path_is_absolute); \
  RUN(test_path_relative); \
  RUN(test_path_split); \
  RUN(test_path_canonical_sa); \
  RUN(test_path_absolute_sa); \
  RUN(test_path_collapse)

TESTS(path) { RUN_PATH_TESTS(); }
