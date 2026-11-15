#include "unit_test.h"
#include "../lib/pathmap.h"
#include "../lib/stralloc.h"
#include "../lib/str.h"

#define MINGW "/usr/x86_64-w64-mingw32/sys-root/mingw"

/* applies m to s and compares with want, returns the pathmap_apply result */
static int
pathmap_check(const pathmap* m, const char* s, const char* want, int* ok) {
  stralloc sa;
  int r;

  stralloc_init(&sa);
  r = pathmap_apply(m, s, str_len(s), &sa);
  stralloc_nul(&sa);
  *ok = !str_diff(sa.s, want);
  stralloc_free(&sa);
  return r;
}

/* libpq.pc, libarchive.pc: prefix=/mingw64 */
TEST(test_pathmap_foreign_prefix) {
  pathmap m = {{0}};
  int ok;

  ASSERT_EQ(1, pathmap_addb(&m, "/mingw64=" MINGW, str_len("/mingw64=" MINGW), 0));
  ASSERT_EQ(1, pathmap_check(&m, "/mingw64", MINGW, &ok));
  ASSERT_EQ(1, ok);
  ASSERT_EQ(1, pathmap_check(&m, "/mingw64/lib", MINGW "/lib", &ok));
  ASSERT_EQ(1, ok);
  ASSERT_EQ(0, pathmap_check(&m, "/mingw64x/lib", "/mingw64x/lib", &ok));
  ASSERT_EQ(1, ok);
  ASSERT_EQ(0, pathmap_check(&m, "${prefix}/lib", "${prefix}/lib", &ok));
  ASSERT_EQ(1, ok);
}

/* libmagic.pc: prefix=/usr/x86_64-w64-mingw32/sysroot/mingw (typo) */
TEST(test_pathmap_fix_typo) {
  pathmap m = {{0}};
  const char* spec = "/usr/x86_64-w64-mingw32/sysroot/=" "/usr/x86_64-w64-mingw32/sys-root/";
  int ok;

  ASSERT_EQ(1, pathmap_addb(&m, spec, str_len(spec), 0));
  ASSERT_EQ(1, pathmap_check(&m, "/usr/x86_64-w64-mingw32/sysroot/mingw", MINGW, &ok));
  ASSERT_EQ(1, ok);
  ASSERT_EQ(0, pathmap_check(&m, MINGW, MINGW, &ok));
  ASSERT_EQ(1, ok);
}

/* a root is prepended per prefix, and only once */
TEST(test_pathmap_root) {
  pathmap m = {{0}};
  const char* spec = "/opt=/sr";
  int ok;

  ASSERT_EQ(1, pathmap_addb(&m, spec, str_len(spec), 1));
  ASSERT_EQ(1, pathmap_check(&m, "/opt/lib", "/sr/opt/lib", &ok));
  ASSERT_EQ(1, ok);
  ASSERT_EQ(0, pathmap_check(&m, "/sr/opt/lib", "/sr/opt/lib", &ok));
  ASSERT_EQ(1, ok);
  ASSERT_EQ(0, pathmap_check(&m, "/usr/lib", "/usr/lib", &ok));
  ASSERT_EQ(1, ok);
  ASSERT_EQ(0, pathmap_addb(&m, "nofrom", 6, 0));
}

TEST(test_pathmap_flags) {
  pathmap m = {{0}};
  stralloc sa;
  const char* in = "-L/mingw64/lib -lpq -I/mingw64/include -DX=/mingw64 /mingw64/a.a";

  stralloc_init(&sa);
  ASSERT_EQ(1, pathmap_addlist(&m, "/mingw64=/r", 0));
  ASSERT_EQ(2, pathmap_flags(&m, in, str_len(in), &sa));
  stralloc_nul(&sa);
  ASSERT_STR_EQUAL(sa.s, "-L/r/lib -lpq -I/r/include -DX=/mingw64 /mingw64/a.a");
  stralloc_free(&sa);
}

#define RUN_PATHMAP_TESTS() \
  RUN(test_pathmap_foreign_prefix); \
  RUN(test_pathmap_fix_typo); \
  RUN(test_pathmap_root); \
  RUN(test_pathmap_flags)

TESTS(pathmap) { RUN_PATHMAP_TESTS(); }
