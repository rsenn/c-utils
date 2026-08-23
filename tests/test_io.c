#include "unit_test.h"
#include "../lib/io.h"
#include "../lib/taia.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*
 * int io_pipe(fd_type* pfd);
 * int64 io_tryread(fd_type d, char* buf, int64 len);
 * int64 io_trywrite(fd_type d, const char* buf, int64 len);
 */
TEST(test_io_pipe_tryread_trywrite) {
  fd_type fds[2];
  char buf[16];

  ASSERT_NE(0, io_pipe(fds));
  ASSERT_EQ(5, io_trywrite(fds[1], "hello", 5));
  ASSERT_EQ(5, io_tryread(fds[0], buf, sizeof(buf)));
  ASSERT_EQ(0, memcmp(buf, "hello", 5));

  io_close(fds[0]);
  io_close(fds[1]);
}

/*
 * int io_socketpair(fd_type* pfd);
 */
TEST(test_io_socketpair) {
  fd_type fds[2];
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
  fd_type fds[2];
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
  fd_type fds[2];

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
  fd_type fds[2];

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
  fd_type fds[2];

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
  fd_type fds[2];

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
  fd_type fds[2];
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
  fd_type fds[2];
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
  fd_type fds[2];

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
 * int io_readfile(fd_type* pfd, const char* s);
 * int io_createfile(fd_type* pfd, const char* s);
 * int io_appendfile(fd_type* pfd, const char* s);
 * int io_readwritefile(fd_type* pfd, const char* s);
 */
TEST(test_io_file_open_variants) {
  fd_type fd;
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

#define RUN_IO_TESTS() \
  RUN(test_io_pipe_tryread_trywrite); \
  RUN(test_io_socketpair); \
  RUN(test_io_waitread_waitwrite); \
  RUN(test_io_wantread_flow); \
  RUN(test_io_wantwrite_flow); \
  RUN(test_io_nonblock_block); \
  RUN(test_io_closeonexec); \
  RUN(test_io_cookie); \
  RUN(test_io_timeout); \
  RUN(test_io_eagain); \
  RUN(test_io_file_open_variants)

TESTS(io) { RUN_IO_TESTS(); }
