#include "unit_test.h"
#include "../lib/buffer.h"
#include "../lib/byte.h"
#include "../lib/path.h"
#include <unistd.h>
#include <stdlib.h>
#include "../lib/str.h"
#include "../lib/stralloc.h"

/*
 * int path_isabs(const char* p, size_t n); int path_isrel(const char* p, size_t n);
 */
TEST(test_path_is_absolute) {
  ASSERT_EQ(1, path_isabs("/test/path", 10));
  ASSERT_EQ(0, path_isabs("test/path", 9));
  ASSERT_EQ(1, path_isabs("//", 2));
  ASSERT_EQ(1, path_isabs("//a//b//", 8));
  ASSERT_EQ(0, path_isabs("", 0));
  ASSERT_EQ(1, path_isrel("", 0));
  ASSERT_EQ(0, path_isrel("/a", 2));
  ASSERT_EQ(0, path_isabs("/a", 0));
}

/*
 * size_t path_next(const char* p, size_t n, size_t pos);
 * size_t path_parentlen(const char* p, size_t n);
 */
TEST(test_path_iterate) {
  size_t k, steps = 0;

  ASSERT_EQ(2, path_next("/a//b/", 6, 0));
  ASSERT_EQ(5, path_next("/a//b/", 6, 2));
  ASSERT_EQ(0, path_next("/a//b/", 6, 5));
  ASSERT_EQ(0, path_next("/", 1, 0));

  for(k = path_parentlen("/a/b/c", 6); k; k = path_parentlen("/a/b/c", k))
    ++steps;

  ASSERT_EQ(3, steps); /* "/a/b", "/a", "/" */
  ASSERT_EQ(3, path_parentlen("a/b/c", 5));
  ASSERT_EQ(1, path_parentlen("a//b/", 5));
  ASSERT_EQ(0, path_parentlen("a", 1));
  ASSERT_EQ(0, path_parentlen("/", 1));
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
  ASSERT_EQ(2, path_rootlen("//x", 3));
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
 * int path_resolve(const char* p, size_t n, stralloc* sa, int symbolic, const char* cwd, size_t cwdn);
 */
TEST(test_path_resolve) {
  stralloc sa;
  stralloc_init(&sa);

  ASSERT_EQ(1, path_resolve("/test/path/test1/test2/../..", 28, &sa, 0, 0, 0));
  ASSERT_EQ(0, stralloc_diffs(&sa, "/test/path"));
  ASSERT_EQ(1, path_resolve("x/../y", 6, &sa, 1, "/a", 2));
  ASSERT_EQ(0, stralloc_diffs(&sa, "/a/y"));
  ASSERT_EQ(1, path_resolve("//tmp///", 8, &sa, 1, 0, 0));
  ASSERT_EQ(0, stralloc_diffs(&sa, "/tmp"));
  stralloc_free(&sa);
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

TEST(test_path_fnmatch) {
  ASSERT_EQ(0, path_fnmatch("*.c", 3, "a.c", 3, 0));
  ASSERT_EQ(PATH_FNM_NOMATCH, path_fnmatch("*.c", 3, "a.h", 3, 0));
  ASSERT_EQ(0, path_fnmatch("a?c", 3, "abc", 3, 0));
  ASSERT_EQ(PATH_FNM_NOMATCH, path_fnmatch("*", 1, "a/b", 3, PATH_FNM_PATHNAME)); /* * does not cross a slash */
  ASSERT_EQ(0, path_fnmatch("*", 1, "a/b", 3, 0));
  ASSERT_EQ(PATH_FNM_NOMATCH,
            path_fnmatch("*", 1, ".hidden", 7, PATH_FNM_PERIOD)); /* leading period only matched explicitly */
  ASSERT_EQ(0, path_fnmatch(".*", 2, ".hidden", 7, PATH_FNM_PERIOD));
}

/*
 * void path_getcwd(stralloc* sa);
 */

TEST(test_path_getcwd) {
  stralloc sa;
  char* cwd;

  stralloc_init(&sa);
  ASSERT_EQ(1, path_getcwd(&sa) > 0);
  ASSERT_EQ(1, sa.len > 0 && sa.s[0] == PATHSEP_C);
  stralloc_nul(&sa);
  cwd = getcwd(NULL, 0);
  ASSERT_STR_EQUAL(cwd, sa.s);
  free(cwd);
  stralloc_free(&sa);
}

/*
 * char* path_gethome(int uid);
 */

TEST(test_path_gethome) {
  char* h = path_gethome(0); /* uid 0 */

  ASSERT_EQ(1, h != NULL);
  ASSERT_EQ(1, h[0] == '/');
}

/*
 * int path_readlink(const char* path, stralloc* sa);
 */

TEST(test_path_readlink) {
  stralloc sa;

  stralloc_init(&sa);
  unlink("/tmp/test_path_unit.lnk");
  ASSERT_EQ(0, symlink("/etc", "/tmp/test_path_unit.lnk"));
  ASSERT_EQ(4, path_readlink("/tmp/test_path_unit.lnk", &sa));
  ASSERT_EQ(1, sa.len == 4 && byte_equal(sa.s, 4, "/etc"));
  ASSERT_EQ(1, path_readlink("/etc", &sa) <= 0); /* not a link */
  unlink("/tmp/test_path_unit.lnk");
  stralloc_free(&sa);
}

/*
 * int path_exists(const char* p);
 */

TEST(test_path_exists) {
  ASSERT_EQ(1, path_exists("/etc"));
  ASSERT_EQ(0, path_exists("/this/path/does/not/exist"));
  ASSERT_EQ(1, path_is_directory("/etc"));
  ASSERT_EQ(0, path_is_directory("/etc/passwd"));
  ASSERT_EQ(0, path_is_directory("/this/path/does/not/exist"));
  ASSERT_EQ(1, path_access("/etc", R_OK) != 0);
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

static int
is_exec(const char* p, void* arg) {
  (void)arg;
  return access(p, X_OK) == 0;
}

static int
sa_is(const stralloc* sa, const char* s) {
  return sa->len == str_len(s) && byte_equal(sa->s, sa->len, s);
}

TEST(test_path_cat_canon) {
  char out[64];
  size_t n;
  stralloc sa;

  stralloc_init(&sa);
  n = path_cat(out, sizeof out, "/usr", 4, "lib", 3);
  ASSERT_EQ(8, n);
  ASSERT_EQ(0, byte_diff(out, 8, "/usr/lib"));
  ASSERT_EQ(1, path_cat_sa(&sa, "a/", 2, "/b", 2));
  ASSERT_EQ(1, sa_is(&sa, "/b")); /* an absolute second part wins */

  n = path_canon(out, sizeof out, "/a/./b/../c//d/", 14);
  ASSERT_EQ(0, byte_diff(out, n, "/a/c/d"));
  n = path_canon(out, sizeof out, "../x/./y", 8);
  ASSERT_EQ(0, byte_diff(out, n, "../x/y"));
  stralloc_copys(&sa, "/a/b/../c");
  ASSERT_EQ(1, path_canon_sa(&sa) != 0);
  ASSERT_EQ(1, sa_is(&sa, "/a/c"));
  stralloc_free(&sa);
}

TEST(test_path_abs_rel) {
  char out[64];
  size_t n;
  stralloc sa;

  stralloc_init(&sa);
  n = path_abs(out, sizeof out, "x/y", 3, "/home/u", 7);
  ASSERT_EQ(0, byte_diff(out, n, "/home/u/x/y"));
  n = path_abs(out, sizeof out, "/etc", 4, "/home/u", 7);
  ASSERT_EQ(0, byte_diff(out, n, "/etc"));
  n = path_rel(out, sizeof out, "/a/b/c", 6, "/a/d", 4);
  ASSERT_EQ(0, byte_diff(out, n, "../b/c"));
  ASSERT_EQ(1, path_rel_sa(&sa, "/a/b", 4, "/a/b", 4) != 0);
  ASSERT_EQ(1, sa_is(&sa, "."));
  stralloc_zero(&sa);
  ASSERT_EQ(1, path_abs_sa(&sa, "/already/absolute", 17) != 0);
  ASSERT_EQ(1, sa_is(&sa, "/already/absolute"));
  stralloc_free(&sa);
}

TEST(test_path_parts) {
  size_t bl = 0, bl2 = 0;
  const char* b;
  stralloc d;
  char* r;

  stralloc_init(&d);
  b = path_basename("/usr/lib/libx.so", 16, &bl);
  ASSERT_EQ(7, bl);
  ASSERT_EQ(0, byte_diff(b, 7, "libx.so"));
  ASSERT_STR_EQUAL("libx.so", path_basename_s("/usr/lib/libx.so"));
  ASSERT_EQ(7, path_extoff("a/b.tar.gz", 10));
  ASSERT_EQ(5, path_extoff("noext", 5)); /* no extension: the length */

  ASSERT_EQ(1, path_isname("file", 4));
  ASSERT_EQ(0, path_isname("a/b", 3));
  ASSERT_EQ(1, path_isabs("/x", 2));
  ASSERT_EQ(0, path_isabs("x", 1));
  ASSERT_EQ(1, path_isrel("x", 1));
  ASSERT_EQ(4, path_dirlen("/a/b/c", 6));
  ASSERT_EQ(4, path_parentlen("/a/b/c", 6));
  ASSERT_EQ(1, path_rootlen("/a/b", 4));
  ASSERT_EQ(4, path_common("/a/b/c", 6, "/a/b/d", 6, &bl2));

  r = path_dirname("/a/b/c.txt", 10, &d);
  ASSERT_STR_EQUAL("/a/b", r);
  stralloc_copys(&d, "/x/y");
  ASSERT_STR_EQUAL("/x", path_dirname_sa(&d));
  stralloc_zero(&d);
  ASSERT_STR_EQUAL("/p/q", path_dirname_s("/p/q/r", &d));
  ASSERT_STR_EQUAL("a/b", path_join("a", "b"));
  stralloc_free(&d);
}

TEST(test_path_list_search) {
  const char* l = "/bin:/usr/bin:/sbin";
  const char* e;
  size_t pos, el, count = 0;
  char out[64];
  stralloc sa;

  stralloc_init(&sa);
  ASSERT_EQ(4, path_listlen("/bin:/usr/bin", 13));
  ASSERT_EQ(5, path_listnext("/bin:/usr/bin", 13, 0));
  PATH_LIST_FOREACH(l, 19, e, el, pos) {
    ++count;
    if(count == 2) {
      ASSERT_EQ(8, el);
      ASSERT_EQ(0, byte_diff(e, el, "/usr/bin"));
    }
  }
  ASSERT_EQ(3, count);

  el = path_search(out, sizeof out, "/this/path/does/not/exist:/bin:/usr/bin", 40, "sh", 2, is_exec, NULL);
  ASSERT_EQ(7, el);
  ASSERT_EQ(0, byte_diff(out, 7, "/bin/sh"));
  ASSERT_EQ(1, path_lookup(&sa, "/bin:/usr/bin", "sh"));
  ASSERT_EQ(1, sa_is(&sa, "/bin/sh"));
  stralloc_zero(&sa);
  ASSERT_EQ(0, path_lookup(&sa, "/bin:/usr/bin", "no-such-command-xyz"));
  /* path_find looks for a directory ENTRY with that name in ONE directory */
  ASSERT_EQ(1, path_find("/etc", "passwd", &sa));
  ASSERT_EQ(1, sa_is(&sa, "/etc/passwd"));
  ASSERT_EQ(0, path_find("/etc", "no-such-entry-xyz", &sa));
  stralloc_free(&sa);
}

TEST(test_path_resolve_dots) {
  stralloc sa;

  stralloc_init(&sa);
  ASSERT_EQ(1, path_resolve("/etc/../etc/./", 14, &sa, 0, "/", 1) != 0);
  ASSERT_EQ(1, sa_is(&sa, "/etc"));
  stralloc_free(&sa);
}

#define RUN_PATH_TESTS() \
  RUN(test_path_cat_canon); \
  RUN(test_path_abs_rel); \
  RUN(test_path_parts); \
  RUN(test_path_list_search); \
  RUN(test_path_resolve_dots); \
  RUN(test_path_basename); \
  RUN(test_path_rootlen); \
  RUN(test_path_cat); \
  RUN(test_path_canon); \
  RUN(test_path_dirlen); \
  RUN(test_path_rel); \
  RUN(test_path_abs); \
  RUN(test_path_common); \
  RUN(test_path_extoff); \
  RUN(test_path_resolve); \
  RUN(test_path_iterate); \
  RUN(test_path_dirname); \
  RUN(test_path_fnmatch); \
  RUN(test_path_getcwd); \
  RUN(test_path_gethome); \
  RUN(test_path_readlink); \
  RUN(test_path_exists); \
  RUN(test_path_is_absolute);

TESTS(path) { RUN_PATH_TESTS(); }
