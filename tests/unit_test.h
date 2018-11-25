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

#include "../lib/buffer.h"
#include "../lib/taia.h"
#include "../lib/windoze.h"
#include "../lib/path_internal.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if WINDOWS_NATIVE
#include <io.h>
#include <winsock.h>
#else
#include <unistd.h>
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
  if(unit_test_run(mu_, name, #name) == EXIT_SUCCESS) {                                                                \
    mu_->success++;                                                                                                    \
  } else {                                                                                                             \
    mu_->failure++;                                                                                                    \
    if(muconf_ptr()->x)                                                                                                \
      return;                                                                                                          \
  }

#define TESTLOG_CHAR(c) buffer_putc(muerr, c)
#define TESTLOG_STR(s) buffer_putm_internal(muerr, s, 0), buffer_flush(muerr)
#define TESTLOG_LONG(d) buffer_putlong(muerr, d), buffer_flush(muerr)
#define TESTLOG_DBL(d) buffer_putdouble(muerr, d, 0), buffer_flush(muerr)
#define TESTLOG_FLUSH() buffer_flush(muerr)

#define equal(a, b) ((a) == (b))
#define not_equal(a, b) ((a) != (b))
#define less_than(a, b) ((a) < (b))
#define not_greater(a, b) ((a) <= (b))
#define greater_than(a, b) ((a) > (b))
#define not_less(a, b) ((a) >= (b))

// Assertions
#define ASSERT_STREQUAL(x1, x2) unit_test_assert(mu_, x1, x2, str_equal, !=)
#define ASSERT_STRALLOCEQUALS(x1, x2) unit_test_assert(mu_, x1, x2, stralloc_equals, !=)
#define ASSERT_EQ(x1, x2) unit_test_assert(mu_, x1, x2, equal, !=)
#define ASSERT_NE(x1, x2) unit_test_assert(mu_, x1, x2, not_equal, ==)
#define ASSERT_LT(x1, x2) unit_test_assert(mu_, x1, x2, less_than, >=)
#define ASSERT_LE(x1, x2) unit_test_assert(mu_, x1, x2, not_greater, >)
#define ASSERT_GT(x1, x2) unit_test_assert(mu_, x1, x2, greater_then, >=)
#define ASSERT_GE(x1, x2) unit_test_assert(mu_, x1, x2, not_less, >)

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

#define unit_test_assert(mu_, x1, x2, op, notop)                                                                       \
  if(!op((x1), (x2))) {                                                                                                \
    const char* file = __FILE__;                                                                                       \
    size_t n = str_rfind(file, ".." PATHSEP_S);                                                                        \
    if(file[n]) file += n + 3;                                                                                            \
    buffer_putm_internal((mu_)->faillog,  file, ":", 0);                                                          \
    buffer_putulong((mu_)->faillog, __LINE__);                                                                         \
    buffer_puts((mu_)->faillog, "  Assertion failed: ");                                                          \
    buffer_puts((mu_)->faillog, #x1);                                                                                  \
    buffer_puts((mu_)->faillog, " " #notop " ");                                                                       \
    buffer_puts((mu_)->faillog, #x2);                                                                                  \
    buffer_puts((mu_)->faillog, ")");                                                                                  \
    buffer_putnlflush((mu_)->faillog);                                                                                 \
    (mu_)->failure++;                                                                                                  \
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

int unit_test_run(struct unit_test* mu_, unit_test_func_t func, const char* name);

#ifdef UNIT_TEST_STATIC_FUNCTIONS
#define START() static void unit_test_execute(struct unit_test* mu_)

#define TESTS(name) static void unit_test_execute_##name(struct unit_test* mu_)

static void unit_test_execute(struct unit_test* mu_);

static struct unit_testConf*
muconf_ptr() {
  static struct unit_testConf c = {/*.q =*/FALSE, /*.s =*/FALSE, /*.v =*/FALSE, /*.x =*/FALSE};
  return &c;
}
static buffer* muout = NULL;
static buffer* muerr = NULL;

static void
unit_test_cleanup(struct unit_test* mu_) {
  if(mu_->testlog)
    buffer_close(mu_->testlog);
  if(mu_->faillog)
    buffer_close(mu_->faillog);
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

#else

#define START() void unit_test_execute(struct unit_test* mu_)
#define TESTS(name) void unit_test_execute_##name(struct unit_test* mu_)
#define EXEC(name) unit_test_execute_##name(mu_)

void unit_test_execute(struct unit_test* mu_);

//#define unit_test_main main
int unit_test_main(int argc, char** argv);

struct unit_testConf* muconf_ptr(void);
int unit_test_call(struct unit_test* mu_, unit_test_func_t func);
void unit_test_cleanup(struct unit_test* mu_);
void unit_test_copy(buffer* src, buffer* dst);
bool unit_test_empty(buffer* file);
struct taia* unit_test_gettime(void);
void unit_test_optparse(int argc, char** argv);
buffer* unit_test_tmpfile(buffer* b, char* tmpl);
void unit_test_usage(const char* cmd);

buffer* unit_test_getbuffer(int fileno);

#endif

#endif /* defined(UNIT_TEST_H) */