/* Copyright (c) 2016 Yusaku Kaneta
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include "../lib/taia.h"
#include "../lib/buffer.h"
#include <math.h>
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock.h>
#else
#include <sys/time.h>
#endif

#ifndef TRUE
#define TRUE (~(0))
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef char bool;

// APIs
#define TEST(name) static void name(struct unit_test* mu_)
#define RUN(name)                                                                                                      \
  if(unit_test_run(mu_, name, #name) == EXIT_SUCCESS) {                                                                  \
    mu_->success++;                                                                                                    \
  } else {                                                                                                             \
    mu_->failure++;                                                                                                    \
    if(muconf()->x) return;                                                                                              \
  }
#define TESTLOG(...) buffer_putm_internal(muerr, __VA_ARGS__, 0)

// Assertions
#define ASSERT_EQ(x1, x2) unit_test_assert(mu_, x1, x2, ==, !=)
#define ASSERT_NE(x1, x2) unit_test_assert(mu_, x1, x2, !=, ==)
#define ASSERT_LT(x1, x2) unit_test_assert(mu_, x1, x2, <, >=)
#define ASSERT_LE(x1, x2) unit_test_assert(mu_, x1, x2, <=, >)
#define ASSERT_GT(x1, x2) unit_test_assert(mu_, x1, x2, >, >=)
#define ASSERT_GE(x1, x2) unit_test_assert(mu_, x1, x2, >=, >)

#define ASSERT_STR_EQUAL(s1, s2) ASSERT_EQ(0, str_diff(s1, s2))
#define ASSERT_STR_EQUAL_N(s1, s2, n) ASSERT_EQ(0, str_diffn(s1, s2, n))

#define ASSERT_SA_EQUAL(sa1, sa2) ASSERT_EQ(0, stralloc_diff(sa1, sa2))
#define ASSERT_SA_EQUALS(sa, s) ASSERT_EQ(0, stralloc_diffs(sa, s))
#define ASSERT_SA_EQUALB(sa, x, n) ASSERT_EQ(0, stralloc_diffb(sa, x, n))

#include "../lib/windoze.h"

#if WINDOWS_NATIVE
#define NOCOLOR 1
#endif

// Colors
#ifndef NOCOLOR
#define BOLD(msg) "\033[1m", msg, "\033[0m"
#define PASS(msg) "\033[32m", msg, "\033[0m"
#define FAIL(msg) "\033[31m", msg, "\033[0m"
#define INFO(msg) "\033[34m", msg, "\033[0m"
#else
#define BOLD(msg) msg
#define PASS(msg) msg
#define FAIL(msg) msg
#define INFO(msg) msg
#endif

// Internals
#define unit_test_typespec(code)                                                                                         \
  _Generic((code),\
char *            : "%s"  ,\
signed char       : "%hhd",\
signed short      : "%hd" ,\
signed int        : "%d"  ,\
signed long       : "%ld" ,\
signed long long  : "%lld",\
unsigned char     : "%u"  ,\
unsigned short    : "%hu" ,\
unsigned int      : "%u"  ,\
unsigned long     : "%lu" ,\
unsigned long long: "%llu",\
float             : "%f"  ,\
double            : "%lf" ,\
long double       : "%Lf" ,\
default           : "%p")

#define unit_test_assert(mu_, x1, x2, op, notop)                                                                          \
  if(!((x1)op(x2))) {                                                                                                  \
    buffer_putsflush((mu_)->faillog, "  Assertion failed: ");                                                                    \
    buffer_putm((mu_)->faillog, unit_test_typespec(x1), x1);                                                                  \
    buffer_puts((mu_)->faillog, " " #notop " ");                                                                            \
    buffer_putm((mu_)->faillog, unit_test_typespec(x2), x2);                                                                  \
    buffer_putm((mu_)->faillog, " (",  __buffer__, ":"); buffer_putulong((mu_)->faillog, __LINE__);                                                          \
    buffer_puts((mu_)->faillog, ")");                                                          \
    buffer_putnlflush((mu_)->faillog);                                                          \
    (mu_)->failure++;                                                                                                   \
    return;                                                                                                            \
  }

struct unit_test {
  tai6464 elapsed;
  int success;
  int failure;
  buffer* testlog;
  buffer* faillog;
};
struct unit_testConf {
  bool q;
  bool s;
  bool v;
  bool x;
};
typedef void (*unit_test_func_t)(struct unit_test* mu_);

#ifdef UNIT_TEST_STATIC_FUNCTIONS
#define START() static void unit_test_execute(struct unit_test* mu_)

#define TESTS(name) static void unit_test_execute_ ## name(struct unit_test* mu_)

static void unit_test_execute(struct unit_test* mu_);

static struct unit_testConf* muconf() {
  static struct unit_testConf c = { /*.q =*/ FALSE, /*.s =*/ FALSE, /*.v =*/ FALSE, /*.x =*/  FALSE};
  return &c;
}
static buffer* muout = NULL;
static buffer* muerr = NULL;

static void
unit_test_cleanup(struct unit_test* mu_) {
  if(mu_->testlog) buffer_close(mu_->testlog);
  if(mu_->faillog) buffer_close(mu_->faillog);
}

static struct taia*
unit_test_gettime() {
  static tai6464 t;
  taia_now(&t);
  return &t;
}

static int
unit_test_call(struct unit_test* mu_, unit_test_func_t func) {
  taia6464 start = unit_test_gettime();
  func(mu_);
  taia_sub(&mu_->elapsed, &start, unit_test_gettime());
  return (mu_->failure == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
unit_test_copy(buffer* src, buffer* dst) {
  int c;
  rewind(src);
  for(c = getc(src); c != EOF; c = getc(src)) buffer_putc(dst, c);
}

static bool
unit_test_empty(buffer* file) {
  fseek(file, 0, SEEK_END);
  return ftell(file) == 0;
}

static buffer*
unit_test_tmpfile() {
  buffer* file = tmpfile();
  if(file == NULL) {
    TESTLOG("ERROR: tmpfile failed\n");
    exit(EXIT_FAILURE);
  }
  return file;
}

static int
unit_test_run(struct unit_test* mu_, unit_test_func_t func, const char* name) {
  int rc;
  static struct unit_test run;
  struct unit_test* running = &run;

  run.testlog = unit_test_tmpfile();
  run.faillog = unit_test_tmpfile();

  if(!muconf()->s) {
    //stdout = running->testlog;
    //stderr = running->testlog;
  }

  rc = unit_test_call(running, func);

  if(running->failure == 0) {
    TESTLOG(PASS("."));
    if(muconf()->v) TESTLOG(PASS("  %s\n"), name);
  } else {
    TESTLOG(FAIL("F"));
    if(muconf()->v) TESTLOG(FAIL("  %s\n"), name);
  }

  if(!unit_test_empty(running->faillog)) {
    fprintf(mu_->testlog, FAIL("\nFAILURE") " in " BOLD("%s\n"), name);
    unit_test_copy(running->faillog, mu_->testlog);
  }

  if(!muconf()->q) {
    if(!unit_test_empty(running->testlog)) {
      fprintf(mu_->testlog, INFO("\nCAPTURED STDOUT/STDERR") " for " BOLD("%s\n"), name);
      unit_test_copy(running->testlog, mu_->testlog);
    }
  }

  unit_test_cleanup(running);
  return rc;
}

static void
unit_test_usage(const char* cmd) {
  TESTLOG("usage: %s [-qsvx]\n"
          "Options:\n"
          "  -q  Quiet stdout.\n"
          "  -s  Disable to capture stdout.\n"
          "  -v  Enalbe verbose mode.\n"
          "  -x  Exit on first failure.\n",
          cmd);
}

static void
unit_test_optparse(int argc, char** argv) {
  size_t i, j;
  for(i = 1; i < (size_t)argc; i++) {
    if(argv[i][0] != '-') {
      unit_test_usage(argv[0]);
      exit(EXIT_FAILURE);
    }

    for(j = 1; j < strlen(argv[i]); j++) {
      switch(argv[i][j]) {
        case 's': muconf()->s = TRUE; break;
        case 'v': muconf()->v = TRUE; break;
        case 'x': muconf()->x = TRUE; break;
        case 'q': muconf()->q = TRUE; break;
        case 'h': unit_test_usage(argv[0]); exit(EXIT_SUCCESS);
        default:
          TESTLOG("%s: illegal option -- %c\n", argv[0], argv[i][j]);
          unit_test_usage(argv[0]);
          exit(EXIT_FAILURE);
      }
    }
  }
}

int
main(int argc, char** argv) {
  int rc;
  static struct unit_test mu_i;
  struct unit_test* mu_ = &mu_i;

mu_i.testlog =  unit_test_tmpfile();

  muout = stdout;
  muerr = stderr;

  unit_test_optparse(argc, argv);

  rc = unit_test_call(mu_, unit_test_execute);

  if(!muconf()->v) buffer_putnlflush(muerr);
  unit_test_copy(mu_->testlog, muerr);
  TESTLOG("\nRAN " BOLD("%d") " TESTS IN " BOLD("%4.3lf") "s\n", mu_->success + mu_->failure, mu_->elapsed);

  if((mu_->success + mu_->failure) > 0) {
    TESTLOG("\n%s (SUCCESS: " PASS("%d") ", FAILURE: " FAIL("%d") ")\n",
            (mu_->failure == 0) ? PASS("OK") : FAIL("FAILED"),
            mu_->success,
            mu_->failure);
  } else {
    TESTLOG(FAIL("\nNO TESTS FOUND\n"));
  }

  unit_test_cleanup(mu_);
  buffer_flush(muerr);
  buffer_flush(muout);
  return rc;
}
#else

#define START() void unit_test_execute(struct unit_test* mu_)
#define TESTS(name) void unit_test_execute_ ## name(struct unit_test* mu_)
#define EXEC(name) unit_test_execute_ ## name(mu_)

void unit_test_execute(struct unit_test* mu_);

int                   main(int argc, char** argv);
struct unit_testConf* muconf(void);
int                   unit_test_call(struct unit_test* mu_, unit_test_func_t func);
void                  unit_test_cleanup(struct unit_test* mu_);
void                  unit_test_copy(buffer* src, buffer* dst);
bool                  unit_test_empty(buffer* file);
struct taia*          unit_test_gettime(void);
void                  unit_test_optparse(int argc, char** argv);
int                   unit_test_run(struct unit_test* mu_, unit_test_func_t func, const char* name);
buffer*               unit_test_tmpfile(void);
void                  unit_test_usage(const char* cmd);

extern buffer* muout;
extern buffer* muerr;

#endif

#endif /* defined(UNIT_TEST_H) */