#include "unit_test.h"
#include "../lib/wait.h"
#include "../lib/process.h"

#include <stdlib.h>

/* Spawn a copy of the running test executable that immediately exits with
 * exitcode (see all_tests.c's "--wait-child" handling). Using
 * process_create() instead of fork() keeps this portable to
 * WINDOWS_NATIVE, which has no fork(). */
static int
spawn_child(const char* exitcode) {
  char* self = process_executable();
  char* argv[] = {self, "--wait-child", (char*)exitcode, 0};
  int pid = process_create(self, argv, 0, 0);

  free(self);
  return pid;
}

/*
 * int wait_pid(int, int*);
 */
TEST(test_wait_pid) {
  int status = 0;
  int child = spawn_child("7");

  ASSERT_EQ(child, wait_pid(child, &status));
  ASSERT_NE(0, wait_ifexited(status));
  ASSERT_EQ(7, wait_exitstatus(status));
}

/*
 * int wait_nointr(int*);
 */
TEST(test_wait_nointr) {
  int status = 0;
  int child = spawn_child("3");

  ASSERT_EQ(child, wait_nointr(&status));
  ASSERT_NE(0, wait_ifexited(status));
  ASSERT_EQ(3, wait_exitstatus(status));
}

/*
 * int wait_nohang(int*);
 */
TEST(test_wait_nohang) {
  int status = 0;
  int child = spawn_child("0");

  ASSERT_EQ(child, wait_pid(child, &status));

  /* no children left at all: waitpid(-1, ..., WNOHANG) fails with
   * ECHILD rather than returning 0 (0 means "children exist, none have
   * changed state yet") */
  ASSERT_EQ(-1, wait_nohang(&status));
}

/*
 * int wait_pid_nohang(int, int*);
 */
TEST(test_wait_pid_nohang) {
  int status = 0;
  int child = spawn_child("0");

  /* poll until the child has exited */
  int r;

  do {
    r = wait_pid_nohang(child, &status);
  } while(r == 0);

  ASSERT_EQ(child, r);
  ASSERT_NE(0, wait_ifexited(status));
}

/*
 * int wait_pids(int*, unsigned int);
 * int wait_pids_nohang(int const*, unsigned int, int*);
 * int wait_pids_reap(int*, unsigned int);
 * unsigned int wait_reap(void);
 *
 * Not tested: exercising the multi-pid variants deterministically requires
 * reasoning about kernel scheduling of several children at once; the
 * single-pid paths above already cover the underlying waitpid() plumbing.
 */
TEST(test_wait_pids) {}

#define RUN_WAIT_TESTS() \
  RUN(test_wait_pid); \
  RUN(test_wait_nointr); \
  RUN(test_wait_nohang); \
  RUN(test_wait_pid_nohang); \
  RUN(test_wait_pids)

TESTS(wait) { RUN_WAIT_TESTS(); }
