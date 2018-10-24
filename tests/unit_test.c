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

#include "../lib/windoze.h"
#include "../lib/buffer.h"
#include "../lib/open.h"
#include "unit_test.h"

#ifndef UNIT_TEST_STATIC_FUNCTIONS

struct unit_testConf*
muconf() {
  static struct unit_testConf c = {/*.q =*/FALSE, /*.s =*/FALSE, /*.v =*/FALSE, /*.x =*/FALSE};
  return &c;
}
static buffer *muout, *muerr;

buffer*
unit_test_getbuffer(int fileno) {
  return fileno == 1 ? muout : muerr;
}

void
unit_test_cleanup(struct unit_test* mu_) {
  if(mu_->testlog) buffer_close(mu_->testlog);
  if(mu_->faillog) buffer_close(mu_->faillog);
}

/*struct taia*
unit_test_gettime() {
  static tai6464 t;
  taia_now(&t);
  return &t;
}*/

int
unit_test_call(struct unit_test* mu_, unit_test_func_t func) {
  struct taia start, end;
  taia_now(&start);
  func(mu_);
  taia_now(&end);
  taia_sub(&mu_->elapsed, &end, &start);

  return (mu_->failure == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void
unit_test_copy(buffer* src, buffer* dst) {
  buffer_copy(dst, src);
}

bool
unit_test_empty(buffer* file) {
  fseek(file, 0, SEEK_END);
  return ftell(file) == 0;
}

buffer*
unit_test_tmpfile(buffer* b, char* tmpl) {
  int fd;
  if((fd = open_temp(tmpl)) == -1) {
    buffer_puts(muerr, "ERROR: tmpfile failed");
    buffer_putnlflush(muerr);
    exit(EXIT_FAILURE);
  }
  buffer_free(b);
  buffer_init(b, write, fd, malloc(1024), 1024);
  return b;
}

int
unit_test_run(struct unit_test* mu_, unit_test_func_t func, const char* name) {
  int rc;
  static struct unit_test run;
  struct unit_test* running = &run;
  static buffer testtmp, failtmp;

  run.testlog = unit_test_tmpfile(&testtmp, "testlog-XXXXXX");
  run.faillog = unit_test_tmpfile(&failtmp, "faillog-XXXXXX");

  if(!muconf()->s) {
    buffer_1small = running->testlog;
    buffer_2 = running->testlog;
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
    buffer_putmflush(mu_->testlog, FAIL("\nFAILURE"), " in ", BOLD(name), "\n");
    unit_test_copy(running->faillog, mu_->testlog);
  }

  if(!muconf()->q) {
    if(!unit_test_empty(running->testlog)) {
      buffer_putmflush(mu_->testlog, INFO("\nCAPTURED STDOUT/STDERR"), " for ", BOLD(name), "\n");
      unit_test_copy(running->testlog, mu_->testlog);
    }
  }

  unit_test_cleanup(running);
  return rc;
}

void
unit_test_usage(const char* cmd) {
  TESTLOG("usage: %s [-qsvx]\n"
          "Options:\n"
          "  -q  Quiet stdout.\n"
          "  -s  Disable to capture stdout.\n"
          "  -v  Enalbe verbose mode.\n"
          "  -x  Exit on first failure.\n",
          cmd);
}

void
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
  static buffer testtmp;

  muout = buffer_1small;
  muerr = buffer_2;

  mu_i.testlog = unit_test_tmpfile(&testtmp, "test-XXXXXX");

#if WINDOWS_NATIVE || defined(_MSC_VER)
  muerr = muout;
#endif

  unit_test_optparse(argc, argv);

  rc = unit_test_call(mu_, unit_test_execute);

  if(!muconf()->v) buffer_putc(muerr, '\n');

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
  return rc;
}
#endif /* !defined(UNIT_TEST_STATIC_FUNCTIONS) */