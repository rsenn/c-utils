#include "unit_test.h"
#include "../lib/iob.h"
#include "../lib/io.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static int64
test_iob_write_cb(fd_type sfd, const void* buf, uint64 n) {
  return write((int)sfd, buf, n);
}

/*
 * io_batch* iob_new(int hint_entries);
 * void iob_free(io_batch* b);
 */
TEST(test_iob_new) {
  io_batch* b = iob_new(4);

  ASSERT_NE(0, (size_t)b);
  ASSERT_EQ(0, iob_bytesleft(b));

  iob_free(b);
}

/*
 * int iob_addbuf(io_batch* b, const void* buf, uint64 n);
 * uint64 iob_bytesleft(const io_batch* b);
 */
TEST(test_iob_addbuf) {
  io_batch* b = iob_new(0);

  ASSERT_NE(0, iob_addbuf(b, "hello", 5));
  ASSERT_EQ(5, iob_bytesleft(b));
  ASSERT_EQ(1, b->bufs);

  iob_free(b);
}

/*
 * int iob_addbuf_free(io_batch* b, const void* buf, uint64 n);
 */
TEST(test_iob_addbuf_free) {
  io_batch* b = iob_new(0);
  char* buf = malloc(3);

  memcpy(buf, "hi", 3);
  ASSERT_NE(0, iob_addbuf_free(b, buf, 2));
  ASSERT_EQ(2, iob_bytesleft(b));

  iob_free(b); /* frees buf via cleanup callback */
}

/*
 * int iob_adds(io_batch* b, const char* s);
 */
TEST(test_iob_adds) {
  io_batch* b = iob_new(0);

  ASSERT_NE(0, iob_adds(b, "hello world"));
  ASSERT_EQ(11, iob_bytesleft(b));

  iob_free(b);
}

/*
 * int iob_adds_free(io_batch* b, const char* s);
 */
TEST(test_iob_adds_free) {
  io_batch* b = iob_new(0);
  char* s = malloc(3);

  memcpy(s, "hi", 3);
  ASSERT_NE(0, iob_adds_free(b, s));
  ASSERT_EQ(2, iob_bytesleft(b));

  iob_free(b);
}

/*
 * void iob_reset(io_batch* b);
 */
TEST(test_iob_reset) {
  io_batch* b = iob_new(0);

  iob_adds(b, "hello");
  ASSERT_EQ(5, iob_bytesleft(b));

  iob_reset(b);
  ASSERT_EQ(0, iob_bytesleft(b));
  ASSERT_EQ(0, b->bufs);

  iob_free(b);
}

/*
 * int64 iob_write(fd_type sfd, io_batch* b, io_write_callback cb);
 */
TEST(test_iob_write) {
  fd_type fds[2];
  io_batch* b = iob_new(0);
  char rbuf[32];

  ASSERT_NE(0, io_pipe(fds));

  iob_adds(b, "hello world");
  ASSERT_EQ(11, iob_write(fds[1], b, test_iob_write_cb));
  ASSERT_EQ(0, iob_bytesleft(b));

  ASSERT_EQ(11, read(fds[0], rbuf, sizeof(rbuf)));
  rbuf[11] = 0;
  ASSERT_STR_EQUAL("hello world", rbuf);

  close(fds[0]);
  close(fds[1]);
  iob_free(b);
}

#define RUN_IOB_TESTS() \
  RUN(test_iob_new); \
  RUN(test_iob_addbuf); \
  RUN(test_iob_addbuf_free); \
  RUN(test_iob_adds); \
  RUN(test_iob_adds_free); \
  RUN(test_iob_reset); \
  RUN(test_iob_write)

TESTS(iob) { RUN_IOB_TESTS(); }
