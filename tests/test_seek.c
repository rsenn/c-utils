#include "unit_test.h"
#include "../lib/seek.h"

#include <fcntl.h>
#include <unistd.h>

/*
 * seek_pos seek_cur(fd_type);
 * int seek_set(fd_type, seek_pos pos);
 * seek_pos seek_end(fd_type);
 */
TEST(test_seek_set_cur_end) {
  char path[] = "/tmp/c-utils-test-seek-XXXXXX";
  int fd = mkstemp(path);

  ASSERT_NE(-1, fd);
  ASSERT_EQ(5, write(fd, "abcde", 5));

  ASSERT_EQ(5, seek_end(fd));
  ASSERT_EQ(0, seek_set(fd, 0));
  ASSERT_EQ(0, seek_cur(fd));
  ASSERT_EQ(0, seek_set(fd, 2));
  ASSERT_EQ(2, seek_cur(fd));

  close(fd);
  unlink(path);
}

/*
 * void seek_by_jump(fd_type, int64 amount);
 */
TEST(test_seek_by_jump) {
  char path[] = "/tmp/c-utils-test-seek-jump-XXXXXX";
  int fd = mkstemp(path);

  ASSERT_NE(-1, fd);
  ASSERT_EQ(5, write(fd, "abcde", 5));

  seek_set(fd, 0);
  seek_by_jump(fd, 3);
  ASSERT_EQ(3, seek_cur(fd));

  close(fd);
  unlink(path);
}

/*
 * void seek_by_read(fd_type, int64 amount);
 */
TEST(test_seek_by_read) {
  char path[] = "/tmp/c-utils-test-seek-read-XXXXXX";
  int fd = mkstemp(path);

  ASSERT_NE(-1, fd);
  ASSERT_EQ(5, write(fd, "abcde", 5));

  seek_set(fd, 0);
  seek_by_read(fd, 2);
  ASSERT_EQ(2, seek_cur(fd));

  close(fd);
  unlink(path);
}

/*
 * seek_begin macro
 */
TEST(test_seek_begin) {
  char path[] = "/tmp/c-utils-test-seek-begin-XXXXXX";
  int fd = mkstemp(path);

  ASSERT_NE(-1, fd);
  ASSERT_EQ(5, write(fd, "abcde", 5));

  seek_set(fd, 3);
  ASSERT_EQ(0, seek_begin(fd));
  ASSERT_EQ(0, seek_cur(fd));

  close(fd);
  unlink(path);
}

#define RUN_SEEK_TESTS() \
  RUN(test_seek_set_cur_end); \
  RUN(test_seek_by_jump); \
  RUN(test_seek_by_read); \
  RUN(test_seek_begin)

TESTS(seek) { RUN_SEEK_TESTS(); }
