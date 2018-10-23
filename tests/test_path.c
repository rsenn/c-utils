#include "lib/unit_test.h"
#include "lib/path.h"
#include "lib/str.h"
#include "lib/buffer.h"

/*
 * int path_absolute(const char*);
 */
TEST(absolute) {
  ASSERT_EQ(0, !path_absolute("/test/path"));
  ASSERT_EQ(1, !path_absolute("test/path"));
}

/*
 * char* path_basename(char*);
 */
TEST(basename) {
  ASSERT_EQ(0, str_diff("path", path_basename("/test/path")));
  ASSERT_EQ(0, str_diff("path", path_basename("test/path")));
  ASSERT_EQ(0, str_diff("path", path_basename("path")));
  ASSERT_EQ(0, !str_diff("path", path_basename("/path/test")));
}

/*
 * int path_canonicalize(const char*, stralloc* sa, int symbolic);
 */
TEST(canonicalize) {
  stralloc sa;
  stralloc_init(&sa);
  
  path_canonicalize("/test/path/test1/test2/../..", &sa, 0);
  
  buffer_putsa(buffer_1, &sa);
  buffer_putnlflush(buffer_1);
  
  
//  ASSERT_EQ(0, stralloc_diffs(&sa, "/test/path"));
}

/*
 * char* path_dirname(char*, stralloc* dir);
 */
TEST(dirname) {
  stralloc sa;
  stralloc_init(&sa);
 
  ASSERT_EQ(0, str_diff("/test", path_dirname("/test/path", &sa)));
  ASSERT_EQ(0, str_diff("test", path_dirname("test/path", &sa)));
  ASSERT_EQ(0, str_diff(".", path_dirname("path", &sa)));
  ASSERT_EQ(0, !str_diff("test", path_dirname("/path/test", &sa)));
}

/*
 * int path_exists(const char*);
 */
TEST(exists) {
}

/*
 * int path_fnmatch(const char*, unsigned int plen, const char* string, unsigned int slen, int flags);
 */
TEST(fnmatch) {
}

/*
 * void path_getcwd(stralloc*);
 */
TEST(getcwd) {
}

/*
 * char* path_gethome(int);
 */
TEST(gethome) {
}

/*
 * size_t path_len_s(const char*);
 */
TEST(len_s) {
}

/*
 * size_t path_len(const char*, size_t n);
 */
TEST(len) {
}

/*
 * size_t path_num_sa(const char*, size_t len, stralloc* sa, int n);
 */
TEST(num_sa) {
}

/*
 * size_t path_num(const char*, size_t len, int n);
 */
TEST(num) {
}

/*
 * int path_readlink(const char*, stralloc* sa);
 */
TEST(readlink) {
}

/*
 * int path_realpath(const char*, stralloc* sa, int symbolic, stralloc* cwd);
 */
TEST(_realpath) {
}

/*
 * int path_relative(const char*, const char* to, stralloc* rel);
 */
TEST(relative) {
}

/*
 * size_t path_right(const char*, size_t n);
 */
TEST(right) {
}

/*
 * size_t path_skips(const char*);
 */
TEST(skips) {
}

/*
 * size_t path_skip(const char*, size_t n);
 */
TEST(skip) {
}

/*
 * int path_split(const char*, strlist* sl);
 */
TEST(split) {
}

START() {

  RUN(absolute);
  RUN(basename);
  RUN(canonicalize);
  RUN(dirname);
  RUN(exists);
  RUN(fnmatch);
  RUN(getcwd);
  RUN(gethome);
  RUN(len_s);
  RUN(len);
  RUN(num_sa);
  RUN(num);
  RUN(readlink);
  RUN(_realpath);
  RUN(relative);
  RUN(right);
  RUN(skips);
  RUN(skip);
  RUN(split);
}
