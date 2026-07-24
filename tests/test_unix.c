#include "unit_test.h"
#include "../lib/unix.h"

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

#define RUN_UNIX_TESTS() \
  RUN(test_unix_fnmatch); \
  RUN(test_fnmatch_b); \
  RUN(test_isleap); \
  RUN(test_pipe2); \
  RUN(test_unix_getopt); \
  RUN(test_unix_errno)

TESTS(unix) { RUN_UNIX_TESTS(); }
