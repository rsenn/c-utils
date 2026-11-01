#include "unit_test.h"
#include "../lib/io.h"
#include "../lib/io_internal.h"
#include "../lib/taia.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "../lib/iopause.h"

/*
 * int io_pipe(int64* d);
 * int64 io_tryread(fd_type d, char* buf, int64 len);
 * int64 io_trywrite(fd_type d, const char* buf, int64 len);
 */
TEST(test_io_pipe_tryread_trywrite) {
  int64 fds[2];
  char buf[16];

  ASSERT_NE(0, io_pipe(fds));
  ASSERT_EQ(5, io_trywrite(fds[1], "hello", 5));
  ASSERT_EQ(5, io_tryread(fds[0], buf, sizeof(buf)));
  ASSERT_EQ(0, memcmp(buf, "hello", 5));

  io_close(fds[0]);
  io_close(fds[1]);
}

/*
 * int io_socketpair(int64* d);
 */
TEST(test_io_socketpair) {
  int64 fds[2];
  char buf[16];

  ASSERT_NE(0, io_socketpair(fds));
  ASSERT_EQ(3, io_trywrite(fds[0], "abc", 3));
  ASSERT_EQ(3, io_tryread(fds[1], buf, sizeof(buf)));
  ASSERT_EQ(0, memcmp(buf, "abc", 3));

  io_close(fds[0]);
  io_close(fds[1]);
}

/*
 * int64 io_waitread(fd_type d, char* buf, int64 len);
 * int64 io_waitwrite(fd_type d, const char* buf, int64 len);
 */
TEST(test_io_waitread_waitwrite) {
  int64 fds[2];
  char buf[16];

  ASSERT_NE(0, io_pipe(fds));
  ASSERT_EQ(4, io_waitwrite(fds[1], "data", 4));
  ASSERT_EQ(4, io_waitread(fds[0], buf, sizeof(buf)));
  ASSERT_EQ(0, memcmp(buf, "data", 4));

  io_close(fds[0]);
  io_close(fds[1]);
}

/*
 * void io_wantread(fd_type d);
 * void io_dontwantread(fd_type d);
 * int io_get_wantread(fd_type d);
 * void io_check(void);
 * int64 io_canread(void);
 */
TEST(test_io_wantread_flow) {
  int64 fds[2];

  ASSERT_NE(0, io_pipe(fds));

  io_wantread(fds[0]);
  ASSERT_NE(0, io_get_wantread(fds[0]));

  io_trywrite(fds[1], "x", 1);
  io_check();
  ASSERT_EQ(fds[0], io_canread());

  io_dontwantread(fds[0]);
  ASSERT_EQ(0, io_get_wantread(fds[0]));

  io_close(fds[0]);
  io_close(fds[1]);
}

/*
 * void io_wantwrite(fd_type d);
 * void io_dontwantwrite(fd_type d);
 * int io_get_wantwrite(fd_type d);
 * int64 io_canwrite(void);
 */
TEST(test_io_wantwrite_flow) {
  int64 fds[2];

  ASSERT_NE(0, io_pipe(fds));

  io_wantwrite(fds[1]);
  ASSERT_NE(0, io_get_wantwrite(fds[1]));

  io_check();
  ASSERT_EQ(fds[1], io_canwrite());

  io_dontwantwrite(fds[1]);
  ASSERT_EQ(0, io_get_wantwrite(fds[1]));

  io_close(fds[0]);
  io_close(fds[1]);
}

/*
 * void io_nonblock(fd_type d);
 * void io_block(fd_type d);
 */
TEST(test_io_nonblock_block) {
  int64 fds[2];

  ASSERT_NE(0, io_pipe(fds));

  io_nonblock(fds[0]);
#if !WINDOWS_NATIVE
  ASSERT_NE(0, fcntl(fds[0], F_GETFL) & O_NONBLOCK);
#endif

  io_block(fds[0]);
#if !WINDOWS_NATIVE
  ASSERT_EQ(0, fcntl(fds[0], F_GETFL) & O_NONBLOCK);
#endif

  io_close(fds[0]);
  io_close(fds[1]);
}

/*
 * void io_closeonexec(fd_type d);
 */
TEST(test_io_closeonexec) {
  int64 fds[2];

  ASSERT_NE(0, io_pipe(fds));

  io_closeonexec(fds[0]);
#if !WINDOWS_NATIVE
  ASSERT_NE(0, fcntl(fds[0], F_GETFD) & FD_CLOEXEC);
#endif

  io_close(fds[0]);
  io_close(fds[1]);
}

/*
 * void io_setcookie(fd_type d, void* cookie);
 * void* io_getcookie(fd_type d);
 */
TEST(test_io_cookie) {
  int64 fds[2];
  int value = 42;

  ASSERT_NE(0, io_pipe(fds));

  io_setcookie(fds[0], &value);
  ASSERT_EQ((size_t)&value, (size_t)io_getcookie(fds[0]));

  io_close(fds[0]);
  io_close(fds[1]);
}

/*
 * void io_timeout(fd_type d, tai6464 t);
 * int io_timedout(fd_type d);
 * int64 io_timeouted(void);
 */
TEST(test_io_timeout) {
  int64 fds[2];
  tai6464 past;

  ASSERT_NE(0, io_pipe(fds));

  taia_now(&past);
  past.sec.x -= 100; /* a timeout 100 seconds in the past */
  io_timeout(fds[0], past);

  ASSERT_NE(0, io_timedout(fds[0]));

  io_close(fds[0]);
  io_close(fds[1]);
}

/*
 * void io_eagain_read(fd_type d);
 * void io_eagain_write(fd_type d);
 */
TEST(test_io_eagain) {
  int64 fds[2];

  ASSERT_NE(0, io_pipe(fds));

  io_wantread(fds[0]);
  io_trywrite(fds[1], "x", 1);
  io_check();
  ASSERT_EQ(fds[0], io_canread());

  /* pretend the read attempt got EAGAIN; must not crash and must clear
   * the "known readable" flag until the next real event */
  io_eagain_read(fds[0]);
  io_eagain_write(fds[1]);

  io_dontwantread(fds[0]);
  io_close(fds[0]);
  io_close(fds[1]);
}

/*
 * int io_readfile(int64* d, const char* s);
 * int io_createfile(int64* d, const char* s);
 * int io_appendfile(int64* d, const char* s);
 * int io_readwritefile(int64* d, const char* s);
 */
TEST(test_io_file_open_variants) {
  int64 fd;
  char path[] = "/tmp/c-utils-test-io-XXXXXX";
  int tmpfd = mkstemp(path);
  char buf[16];

  ASSERT_NE(-1, tmpfd);
  close(tmpfd);

  ASSERT_NE(0, io_createfile(&fd, path));
  ASSERT_EQ(5, io_trywrite(fd, "abcde", 5));
  io_close(fd);

  ASSERT_NE(0, io_readfile(&fd, path));
  ASSERT_EQ(5, io_tryread(fd, buf, sizeof(buf)));
  ASSERT_EQ(0, memcmp(buf, "abcde", 5));
  io_close(fd);

  ASSERT_NE(0, io_appendfile(&fd, path));
  ASSERT_EQ(3, io_trywrite(fd, "fgh", 3));
  io_close(fd);

  ASSERT_NE(0, io_readwritefile(&fd, path));
  ASSERT_EQ(8, io_tryread(fd, buf, sizeof(buf)));
  ASSERT_EQ(0, memcmp(buf, "abcdefgh", 8));
  io_close(fd);

  unlink(path);
}

/*
 * int io_passfd(fd_type sock, fd_type fd);
 * int64 io_receivefd(fd_type sock);
 */
TEST(test_io_passfd_receivefd) {
  int64 sv[2], p[2];
  int64 fd;
  char buf[8];

  ASSERT_NE(0, io_socketpair(sv));
  ASSERT_NE(0, io_pipe(p));

  /* the received descriptor is a new one for the same pipe */
  ASSERT_EQ(0, io_passfd(sv[0], p[1]));
  fd = io_receivefd(sv[1]);
  ASSERT_GT(fd, 2);
  ASSERT_NE(p[1], fd);

  ASSERT_EQ(3, write((int)fd, "abc", 3));
  ASSERT_EQ(3, read(p[0], buf, sizeof(buf)));
  ASSERT_EQ(0, memcmp(buf, "abc", 3));

  /* a plain byte without a descriptor is a protocol error */
  ASSERT_EQ(1, write(sv[0], "x", 1));
  ASSERT_EQ(-1, io_receivefd(sv[1]));

  close((int)fd);
  io_close(p[0]);
  io_close(p[1]);
  io_close(sv[0]);
  io_close(sv[1]);
}

static int64
write_cb(fd_type fd, const void* buf, uint64 n) {
  return write(fd, buf, n);
}

/*
 * io_dup, io_isnonblock, io_eagain, io_sigpipe, io_debugstring, io_tryreadtimeout,
 * io_trywritetimeout, io_waituntil, io_waituntil2, io_wait, io_onlywantread, io_onlywantwrite
 */
TEST(test_io_dup_flags_wait) {
  int64 fds[2], copy;
  char buf[256], c;
  tai6464 t;
  int64 r;

  ASSERT_NE(0, io_pipe(fds));

  copy = io_dup(fds[0]);
  ASSERT_EQ(1, copy >= 0 && copy != fds[0]);
  io_close(copy);

  io_nonblock(fds[0]);
  ASSERT_EQ(1, io_isnonblock(fds[0]));
  io_block(fds[0]);
  ASSERT_EQ(0, io_isnonblock(fds[0]));
  io_nonblock(fds[0]);

  io_sigpipe(); /* ignores SIGPIPE: writing to a closed pipe is an error, not a signal */

  /* nothing to read yet and no timeout set: -1, not -2 */
  r = io_tryreadtimeout(fds[0], &c, 1);
  ASSERT_EQ(-1, r);

  /* after the timeout has passed the answer is -2 and ETIMEDOUT */
  taia_now(&t);
  t.sec.x -= 1;
  io_timeout(fds[0], t);
  r = io_tryreadtimeout(fds[0], &c, 1);
  ASSERT_EQ(-2, r);
  ASSERT_EQ(ETIMEDOUT, errno);

  ASSERT_EQ(1, io_trywritetimeout(fds[1], "x", 1));
  ASSERT_EQ(1, io_tryreadtimeout(fds[0], &c, 1));
  ASSERT_EQ('x', c);

  /* the debug text names the descriptor */
  ASSERT_EQ(1, io_debugstring(fds[0], buf, sizeof buf) > 0);
  ASSERT_EQ(0, io_debugstring(fds[0], buf, 10)); /* too small */

  /* waiting with nothing wanted returns at the deadline */
  io_dontwantread(fds[0]);
  io_dontwantwrite(fds[0]);
  r = io_waituntil2(10);
  ASSERT_EQ(1, r <= 0);
  taia_now(&t);
  t.sec.x -= 1;
  io_waituntil(t); /* a deadline in the past: does not block */

  io_onlywantread(fds[0]);
  io_trywrite(fds[1], "y", 1);
  ASSERT_EQ(1, io_wait() >= 0);
  ASSERT_EQ(fds[0], io_canread());
  io_onlywantwrite(fds[1]);
  ASSERT_EQ(1, io_waituntil2(1000) >= 0);
  ASSERT_EQ(fds[1], io_canwrite());

  io_eagain(fds[1]);
  io_dontwantread(fds[0]);
  io_dontwantwrite(fds[1]);
  io_close(fds[0]);
  io_close(fds[1]);
}

/*
 * io_sendfile, io_mmapwritefile
 */
TEST(test_io_sendfile_mmapwritefile) {
  const char* fn = "/tmp/c-utils-iosend.tmp";
  int fd = open(fn, O_WRONLY | O_CREAT | O_TRUNC, 0600);
  int64 sv[2];
  char got[32];
  int64 r;
  int in;

  write(fd, "0123456789abcdef", 16);
  close(fd);
  in = open(fn, O_RDONLY);

  ASSERT_NE(0, io_socketpair(sv));
  r = io_sendfile(sv[0], in, 4, 6);
  ASSERT_EQ(6, r);
  ASSERT_EQ(6, read(sv[1], got, sizeof got));
  ASSERT_EQ(0, memcmp(got, "456789", 6));

  r = io_mmapwritefile(sv[0], in, 10, 6, write_cb);
  ASSERT_EQ(6, r);
  ASSERT_EQ(6, read(sv[1], got, sizeof got));
  ASSERT_EQ(0, memcmp(got, "abcdef", 6));

  close(in);
  io_close(sv[0]);
  io_close(sv[1]);
  unlink(fn);
}

/*
 * iopause
 */
TEST(test_iopause) {
  int p[2];
  iopause_fd x[1];
  struct taia now, deadline;

  ASSERT_EQ(0, pipe(p));
  x[0].fd = p[0];
  x[0].events = IOPAUSE_READ;

  taia_now(&now);
  deadline = now; /* already due: polls */
  iopause(x, 1, &deadline, &now);
  ASSERT_EQ(0, x[0].revents);

  write(p[1], "x", 1);
  taia_now(&now);
  deadline = now;
  deadline.sec.x += 1;
  iopause(x, 1, &deadline, &now);
  ASSERT_EQ(IOPAUSE_READ, x[0].revents & IOPAUSE_READ);
  close(p[0]);
  close(p[1]);
}

#define RUN_IO_TESTS() \
  RUN(test_io_dup_flags_wait); \
  RUN(test_io_sendfile_mmapwritefile); \
  RUN(test_iopause); \
  RUN(test_io_pipe_tryread_trywrite); \
  RUN(test_io_socketpair); \
  RUN(test_io_waitread_waitwrite); \
  RUN(test_io_wantread_flow); \
  RUN(test_io_wantwrite_flow); \
  RUN(test_io_nonblock_block); \
  RUN(test_io_closeonexec); \
  RUN(test_io_passfd_receivefd); \
  RUN(test_io_cookie); \
  RUN(test_io_timeout); \
  RUN(test_io_eagain); \
  RUN(test_io_file_open_variants)

TESTS(io) { RUN_IO_TESTS(); }
