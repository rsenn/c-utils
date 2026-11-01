#include "unit_test.h"
#include "../lib/unix.h"

#include <string.h>
#include <unistd.h>

/*
 * int unix_fnmatch(const char* pattern, const char* string, int flags);
 */
TEST(test_unix_fnmatch) {
  ASSERT_EQ(0, unix_fnmatch("foo*", "foobar", 0));
  ASSERT_NE(0, unix_fnmatch("foo*", "barfoo", 0));
  ASSERT_EQ(0, unix_fnmatch("f?o", "foo", 0));
  ASSERT_EQ(0, unix_fnmatch("*.c", "test.c", 0));
}

/*
 * int fnmatch_b(const char*, size_t, const char*, size_t slen, int flags);
 */
TEST(test_fnmatch_b) {
  ASSERT_EQ(0, fnmatch_b("foo*", 4, "foobar", 6, 0));
  ASSERT_NE(0, fnmatch_b("foo*", 4, "barfoo", 6, 0));
  ASSERT_EQ(0, fnmatch_b("*.c", 3, "test.c", 6, 0));
}

/*
 * int isleap(int year);
 */
TEST(test_isleap) {
  ASSERT_NE(0, isleap(2000));
  ASSERT_NE(0, isleap(2024));
  ASSERT_EQ(0, isleap(1900));
  ASSERT_EQ(0, isleap(2023));
}

/*
 * int pipe2(int fd[2], int flags);
 */
TEST(test_pipe2) {
  int fds[2];

  ASSERT_EQ(0, pipe2(fds, 0));
  ASSERT_EQ(4, write(fds[1], "data", 4));

  char buf[4];
  ASSERT_EQ(4, read(fds[0], buf, 4));
  ASSERT_EQ(0, memcmp(buf, "data", 4));

  close(fds[0]);
  close(fds[1]);
}

/*
 * int unix_getopt(int, char* const[], const char*);
 */
TEST(test_unix_getopt) {
  char arg0[] = "prog";
  char arg1[] = "-a";
  char arg2[] = "-b";
  char* argv[] = {arg0, arg1, arg2, NULL};
  int c;
  int seen_a = 0, seen_b = 0;

  unix_optind = 1;

  while((c = unix_getopt(3, argv, "ab")) != -1) {
    if(c == 'a')
      seen_a = 1;
    else if(c == 'b')
      seen_b = 1;
  }

  ASSERT_NE(0, seen_a);
  ASSERT_NE(0, seen_b);
}

/*
 * const char* unix_errno(int);
 */
TEST(test_unix_errno) {
  const char* msg = unix_errno(0);

  ASSERT_NE(NULL, msg);
}

/*
 * int unix_getopt_long(argc, argv, optstring, longopts, int* index);
 * int unix_getopt_long_only(...)
 */
TEST(test_unix_getopt_long) {
  static int verbose_flag;
  struct unix_longopt longopts[] = {
      {"verbose", 0, &verbose_flag, 1},
      {"file", 1, 0, 'f'},
      {"count", 1, 0, 'c'},
      {0, 0, 0, 0},
  };
  char a0[] = "prog", a1[] = "--verbose", a2[] = "--file=x.txt", a3[] = "-c", a4[] = "7", a5[] = "rest";
  char* argv[] = {a0, a1, a2, a3, a4, a5, NULL};
  int c, idx = -1, got_f = 0, got_c = 0;
  char file[16] = "", count[16] = "";

  unix_optind = 1;
  verbose_flag = 0;
  while((c = unix_getopt_long(6, argv, "f:c:", longopts, &idx)) != -1) {
    if(c == 'f') {
      got_f = 1;
      strncpy(file, unix_optarg, 15);
    } else if(c == 'c') {
      got_c = 1;
      strncpy(count, unix_optarg, 15);
    }
  }
  ASSERT_EQ(1, verbose_flag); /* a flag option sets the variable and returns 0 */
  ASSERT_EQ(1, got_f);
  ASSERT_STR_EQUAL("x.txt", file);
  ASSERT_EQ(1, got_c);
  ASSERT_STR_EQUAL("7", count);
  ASSERT_EQ(5, unix_optind); /* "rest" is the first non option */
}
#define RUN_UNIX_TESTS() \
  RUN(test_unix_getopt_long); \
  RUN(test_unix_fnmatch); \
  RUN(test_fnmatch_b); \
  RUN(test_isleap); \
  RUN(test_pipe2); \
  RUN(test_unix_getopt); \
  RUN(test_unix_errno)

TESTS(unix) { RUN_UNIX_TESTS(); }
