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

#ifndef __MINUNIT_H__
#define __MINUNIT_H__

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

// APIs
#define START() static void unit_test_execute(struct unit_test* mu_)
#define TEST(name) static void name(struct unit_test* mu)
#define RUN(name)                                                                                                      \
  if(unit_test_run(mu_, name, #name) == EXIT_SUCCESS) {                                                                  \
    mu_->success++;                                                                                                    \
  } else {                                                                                                             \
    mu_->failure++;                                                                                                    \
    if(muconf->x) return;                                                                                              \
  }
#define TESTLOG(...) fprintf(muerr, __VA_ARGS__)

// Assertions
#define ASSERT_EQ(x1, x2) unit_test_assert(mu, x1, x2, ==, !=)
#define ASSERT_NE(x1, x2) unit_test_assert(mu, x1, x2, !=, ==)
#define ASSERT_LT(x1, x2) unit_test_assert(mu, x1, x2, <, >=)
#define ASSERT_LE(x1, x2) unit_test_assert(mu, x1, x2, <=, >)
#define ASSERT_GT(x1, x2) unit_test_assert(mu, x1, x2, >, >=)
#define ASSERT_GE(x1, x2) unit_test_assert(mu, x1, x2, >=, >)

// Colors
#define BOLD(msg) "\033[1m" msg "\033[0m"
#define PASS(msg) "\033[32m" msg "\033[0m"
#define FAIL(msg) "\033[31m" msg "\033[0m"
#define INFO(msg) "\033[34m" msg "\033[0m"

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

#define unit_test_assert(mu, x1, x2, op, notop)                                                                          \
  if(!((x1)op(x2))) {                                                                                                  \
    fprintf((mu)->faillog, "  Assertion failed: ");                                                                    \
    fprintf((mu)->faillog, unit_test_typespec(x1), x1);                                                                  \
    fprintf((mu)->faillog, " " #notop " ");                                                                            \
    fprintf((mu)->faillog, unit_test_typespec(x2), x2);                                                                  \
    fprintf((mu)->faillog, " (%s:%d)\n", __FILE__, __LINE__);                                                          \
    (mu)->failure++;                                                                                                   \
    return;                                                                                                            \
  }

struct unit_test {
  double elapsed;
  int success;
  int failure;
  FILE* testlog;
  FILE* faillog;
};
struct unit_testConf {
  bool q;
  bool s;
  bool v;
  bool x;
};
typedef void (*unit_test_func_t)(struct unit_test* mu);
static void unit_test_execute(struct unit_test* mu);

static struct unit_testConf* muconf = &(struct unit_testConf){.q = false, .s = false, .v = false, .x = false};
static FILE* muout = NULL;
static FILE* muerr = NULL;

static void
unit_test_cleanup(struct unit_test* mu) {
  if(mu->testlog) fclose(mu->testlog);
  if(mu->faillog) fclose(mu->faillog);
}

static double
unit_test_gettime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

static int
unit_test_call(struct unit_test* mu, unit_test_func_t func) {
  double start = unit_test_gettime();
  func(mu);
  mu->elapsed = unit_test_gettime() - start;
  return (mu->failure == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
unit_test_copy(FILE* src, FILE* dst) {
  rewind(src);
  for(int c = getc(src); c != EOF; c = getc(src)) fputc(c, dst);
}

static bool
unit_test_empty(FILE* file) {
  fseek(file, 0, SEEK_END);
  return ftell(file) == 0;
}

static FILE*
unit_test_tmpfile() {
  FILE* file = tmpfile();
  if(file == NULL) {
    TESTLOG("ERROR: tmpfile failed\n");
    exit(EXIT_FAILURE);
  }
  return file;
}

static int
unit_test_run(struct unit_test* mu, unit_test_func_t func, const char* name) {
  struct unit_test* running = &(struct unit_test){
      .elapsed = 0, .success = 0, .failure = 0, .testlog = unit_test_tmpfile(), .faillog = unit_test_tmpfile()};

  if(!muconf->s) {
    //stdout = running->testlog;
    //stderr = running->testlog;
  }

  int rc = unit_test_call(running, func);

  if(running->failure == 0) {
    TESTLOG(PASS("."));
    if(muconf->v) TESTLOG(PASS("  %s\n"), name);
  } else {
    TESTLOG(FAIL("F"));
    if(muconf->v) TESTLOG(FAIL("  %s\n"), name);
  }

  if(!unit_test_empty(running->faillog)) {
    fprintf(mu->testlog, FAIL("\nFAILURE") " in " BOLD("%s\n"), name);
    unit_test_copy(running->faillog, mu->testlog);
  }

  if(!muconf->q) {
    if(!unit_test_empty(running->testlog)) {
      fprintf(mu->testlog, INFO("\nCAPTURED STDOUT/STDERR") " for " BOLD("%s\n"), name);
      unit_test_copy(running->testlog, mu->testlog);
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
  for(size_t i = 1; i < (size_t)argc; i++) {
    if(argv[i][0] != '-') {
      unit_test_usage(argv[0]);
      exit(EXIT_FAILURE);
    }

    for(size_t j = 1; j < strlen(argv[i]); j++) {
      switch(argv[i][j]) {
        case 's': muconf->s = true; break;
        case 'v': muconf->v = true; break;
        case 'x': muconf->x = true; break;
        case 'q': muconf->q = true; break;
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
  muout = stdout;
  muerr = stderr;

  unit_test_optparse(argc, argv);

  struct unit_test* mu = &(struct unit_test){.success = 0, .failure = 0, .testlog = unit_test_tmpfile(), .faillog = NULL};
  int rc = unit_test_call(mu, unit_test_execute);

  if(!muconf->v) fputc('\n', muerr);
  unit_test_copy(mu->testlog, muerr);
  TESTLOG("\nRAN " BOLD("%d") " TESTS IN " BOLD("%4.3lf") "s\n", mu->success + mu->failure, mu->elapsed);

  if((mu->success + mu->failure) > 0) {
    TESTLOG("\n%s (SUCCESS: " PASS("%d") ", FAILURE: " FAIL("%d") ")\n",
            (mu->failure == 0) ? PASS("OK") : FAIL("FAILED"),
            mu->success,
            mu->failure);
  } else {
    TESTLOG(FAIL("\nNO TESTS FOUND\n"));
  }

  unit_test_cleanup(mu);
  return rc;
}
#endif

