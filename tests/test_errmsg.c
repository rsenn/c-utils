#include "unit_test.h"
#include "../lib/errmsg.h"
#include "../lib/str.h"
#include "../lib/ndelay.h"
#include <unistd.h>

/* Redirects fd to a pipe, runs fn(), restores fd, and returns whatever
   fn() wrote as a NUL-terminated string in buf (truncated to bufsz-1). */
static size_t
capture_fd(int fd, void (*fn)(void), char* buf, size_t bufsz) {
  int saved = dup(fd);
  int p[2];
  size_t n = 0;

  pipe(p);
  dup2(p[1], fd);
  close(p[1]);

  fn();

  dup2(saved, fd);
  close(saved);

  ndelay_on(p[0]);
  ssize_t r = read(p[0], buf, bufsz - 1);
  if(r > 0)
    n = (size_t)r;
  buf[n] = '\0';
  close(p[0]);

  return n;
}

/*
 * const char* errmsg_iam(const char* who);
 */
TEST(test_errmsg_iam) {
  const char* prev = errmsg_iam("first");

  ASSERT_EQ(0, str_diff(errmsg_iam(NULL), "first"));

  const char* was = errmsg_iam("second");

  ASSERT_EQ(0, str_diff(was, "first"));
  ASSERT_EQ(0, str_diff(errmsg_iam(NULL), "second"));

  errmsg_iam((char*)prev);
}

static void
call_warn(void) {
  errmsg_warn("hello", " ", "world", (char*)0);
}

/*
 * void errmsg_warn(const char* message, ...);
 */
TEST(test_errmsg_warn) {
  char buf[256];
  const char* prev = errmsg_iam(NULL);

  errmsg_iam("");
  capture_fd(2, call_warn, buf, sizeof(buf));
  errmsg_iam((char*)prev);

  ASSERT_NE(0, str_contains(buf, "hello world"));
}

static void
call_warnsys(void) {
  errmsg_warnsys("failed", (char*)0);
}

/*
 * void errmsg_warnsys(const char* message, ...);
 */
TEST(test_errmsg_warnsys) {
  char buf[256];
  const char* prev = errmsg_iam(NULL);

  errmsg_iam("");
  capture_fd(2, call_warnsys, buf, sizeof(buf));
  errmsg_iam((char*)prev);

  ASSERT_NE(0, str_contains(buf, "failed"));
}

static void
call_warnerr(void) {
  errmsg_warnerr(0, "everything ok", (char*)0);
}

/*
 * void errmsg_warnerr(int err, const char* message, ...);
 */
TEST(test_errmsg_warnerr) {
  char buf[256];
  const char* prev = errmsg_iam(NULL);

  errmsg_iam("");
  capture_fd(2, call_warnerr, buf, sizeof(buf));
  errmsg_iam((char*)prev);

  ASSERT_NE(0, str_contains(buf, "everything ok"));
}

static void
call_info(void) {
  errmsg_info("hi there", (char*)0);
}

/*
 * void errmsg_info(const char* message, ...);
 */
TEST(test_errmsg_info) {
  char buf[256];
  const char* prev = errmsg_iam(NULL);

  errmsg_iam("");
  capture_fd(1, call_info, buf, sizeof(buf));
  errmsg_iam((char*)prev);

  ASSERT_NE(0, str_contains(buf, "hi there"));
}

static void
call_infosys(void) {
  errmsg_infosys("done", (char*)0);
}

/*
 * void errmsg_infosys(const char* message, ...);
 */
TEST(test_errmsg_infosys) {
  char buf[256];
  const char* prev = errmsg_iam(NULL);

  errmsg_iam("");
  capture_fd(1, call_infosys, buf, sizeof(buf));
  errmsg_iam((char*)prev);

  ASSERT_NE(0, str_contains(buf, "done"));
}

/*
 * #define carp(...) errmsg_warn(__VA_ARGS__, (char*)0)
 */
static void
call_carp(void) {
  carp("carped");
}

TEST(test_errmsg_carp) {
  char buf[256];
  const char* prev = errmsg_iam(NULL);

  errmsg_iam("");
  capture_fd(2, call_carp, buf, sizeof(buf));
  errmsg_iam((char*)prev);

  ASSERT_NE(0, str_contains(buf, "carped"));
}

/*
 * #define msg(...) errmsg_info(__VA_ARGS__, (char*)0)
 */
static void
call_msg(void) {
  msg("msged");
}

TEST(test_errmsg_msg) {
  char buf[256];
  const char* prev = errmsg_iam(NULL);

  errmsg_iam("");
  capture_fd(1, call_msg, buf, sizeof(buf));
  errmsg_iam((char*)prev);

  ASSERT_NE(0, str_contains(buf, "msged"));
}

#define RUN_ERRMSG_TESTS() \
  RUN(test_errmsg_iam); \
  RUN(test_errmsg_warn); \
  RUN(test_errmsg_warnsys); \
  RUN(test_errmsg_warnerr); \
  RUN(test_errmsg_info); \
  RUN(test_errmsg_infosys); \
  RUN(test_errmsg_carp); \
  RUN(test_errmsg_msg)

TESTS(errmsg) { RUN_ERRMSG_TESTS(); }
