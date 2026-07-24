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
#include "../lib/stralloc.h"
#include "../lib/buffer.h"
#include "../lib/open.h"
#include "../lib/io_internal.h"
#include "../lib/errmsg.h"
#include "../lib/alloc.h"
#include "unit_test.h"

#ifndef UNIT_TEST_STATIC_FUNCTIONS

static struct unit_testConf muconf = {/*.q =*/FALSE, /*.s =*/FALSE, /*.v =*/FALSE, /*.x =*/FALSE};

struct unit_testConf*
muconf_ptr(void) {
  return &muconf;
}
static buffer *muout, *muerr;

buffer*
unit_test_getbuffer(int fileno) {
  return fileno == 1 ? muout : muerr;
}

void
unit_test_cleanup(struct unit_test* mu_) {
  if(mu_->testlog != muout)
    buffer_close(mu_->testlog);

  if(mu_->faillog != muerr)
    buffer_close(mu_->faillog);
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

  buffer_flush(mu_->testlog);
  buffer_flush(mu_->faillog);

  return (mu_->failure == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void
unit_test_copy(buffer* src, buffer* dst) {
  //  buffer_copy(dst, src);
}

bool
unit_test_empty(buffer* b) {
  size_t pos = lseek(b->fd, 0, SEEK_END);
  return pos <= 1;
}

void
unit_test_closetemp(buffer* b) {
  buffer_flush(b);

  if(unit_test_empty(b)) {
    stralloc filename;
    stralloc_init(&filename);

    write(b->fd, "\n", 1);

    open_filename(b->fd, &filename);
    stralloc_nul(&filename);

    close(b->fd);
    b->fd = -1;

    if(filename.len) {
      int ret = unlink(filename.s);

      if(ret)
        errmsg_warnsys("unlink(", filename.s, "): ", 0);
    }
  }
}

buffer*
unit_test_tmpfile(buffer* b, char** tmpl) {
  int fd;

  if((fd = open_temp(tmpl)) == -1) {
    buffer_puts(muerr, "ERROR: tmpfile failed");
    buffer_putnlflush(muerr);
    exit(EXIT_FAILURE);
  }
  buffer_free(b);
  buffer_init(b, (buffer_op_proto*)(void*)&write, fd, malloc(1024), 1024);
  b->deinit = &unit_test_closetemp;
  return b;
}

int
unit_test_run(struct unit_test* mu_, unit_test_func_t func, const char* name) {
  int rc;
  static struct unit_test run;
  struct unit_test* running = &run;
  static buffer testtmp, failtmp;

  static char* testlog_filename;
  static char* faillog_filename;

  if(testlog_filename)
    alloc_free(testlog_filename);

  if(faillog_filename)
    alloc_free(faillog_filename);
  testlog_filename = "testlog-XXXXXX";
  faillog_filename = "faillog-XXXXXX";
  run.testlog = unit_test_tmpfile(&testtmp, &testlog_filename);
  run.faillog = unit_test_tmpfile(&failtmp, &faillog_filename);

  /*if(!muconf_ptr()->s) {
    buffer_1small = running->testlog;
    buffer_2 = running->testlog;
  }*/

  rc = unit_test_call(running, func);

  if(running->failure == 0) {
    buffer_putm_internal(mu_->testlog, PASS("."), NULL);

    if(muconf_ptr()->v) {
      TESTLOG_STR("  ");
      TESTLOG_STR(name);
      TESTLOG_STR("\n");
    }
  } else {
    TESTLOG_STR(FAIL("F"));

    if(muconf_ptr()->v) {
      TESTLOG_STR("  ");
      TESTLOG_STR(name);
      TESTLOG_STR("\n");
    }
  }

  if(!unit_test_empty(running->faillog)) {
    buffer_putm_internal(mu_->testlog, FAIL("\nFAILURE"), " in ", BOLD(name), "\n", NULL);
    unit_test_copy(running->faillog, mu_->testlog);
  }

  if(!muconf_ptr()->q) {
    if(!unit_test_empty(running->testlog)) {
      buffer_putm_internal(mu_->testlog, INFO("\nCAPTURED STDOUT/STDERR"), " for ", BOLD(name), "\n", NULL);
      unit_test_copy(running->testlog, mu_->testlog);
    }
  }

  buffer_flush(mu_->testlog);
  buffer_flush(mu_->faillog);

  unit_test_cleanup(running);
  return rc;
}

void
unit_test_usage(const char* cmd) {
  TESTLOG_STR("usage: %s [-qsvx]\n"
              "Options:\n"
              "  -q  Quiet stdout.\n"
              "  -s  Disable to capture stdout.\n"
              "  -v  Enalbe verbose mode.\n"
              "  -x  Exit on first failure.\n");
  TESTLOG_STR(cmd);
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
        case 's': muconf_ptr()->s = TRUE; break;
        case 'v': muconf_ptr()->v = TRUE; break;
        case 'x': muconf_ptr()->x = TRUE; break;
        case 'q': muconf_ptr()->q = TRUE; break;
        case 'h': unit_test_usage(argv[0]); exit(EXIT_SUCCESS);
        default:
          TESTLOG_STR(argv[0]);
          TESTLOG_STR(": illegal option -- ");
          TESTLOG_CHAR(argv[i][j]);
          unit_test_usage(argv[0]);
          TESTLOG_FLUSH();
          exit(EXIT_FAILURE);
      }
    }
  }
}

int
unit_test_main(int argc, char** argv) {
  int rc;
  static struct unit_test mu_i;
  struct unit_test* mu_ = &mu_i;
  static buffer testtmp;

  muout = buffer_1small;
  muerr = buffer_2;

#if WINDOWS_NATIVE || defined(_MSC_VER)
  muerr = muout;
#endif

  mu_i.testlog = muout; // unit_test_tmpfile(&testtmp, "test-XXXXXX");
  mu_i.faillog = muerr;

  unit_test_optparse(argc, argv);

  rc = unit_test_call(mu_, unit_test_execute);

  if(!muconf_ptr()->v)
    buffer_putc(muerr, '\n');

  unit_test_copy(mu_->testlog, muerr);
  TESTLOG_STR("\nRAN ");
  TESTLOG_LONG(mu_->success + mu_->failure);
  TESTLOG_STR(" TESTS IN ");
  TESTLOG_DBL(tai_approx(&mu_->elapsed.sec) + (double)mu_->elapsed.nano / 1000000000);
  TESTLOG_STR("\n");

  if((mu_->success + mu_->failure) > 0) {
    TESTLOG_STR("\n");
    TESTLOG_STR((mu_->failure == 0) ? PASS("OK") : FAIL("FAILED"));

    TESTLOG_STR(" (SUCCESS: ");
    TESTLOG_LONG(mu_->success);
    TESTLOG_STR(", FAILURE: ");
    TESTLOG_LONG(mu_->failure);
    TESTLOG_STR("\n");

  } else {
    TESTLOG_STR(FAIL("\nNO TESTS FOUND\n"));
  }

  unit_test_cleanup(mu_);
  return rc;
}
#endif /* !defined(UNIT_TEST_STATIC_FUNCTIONS) */
