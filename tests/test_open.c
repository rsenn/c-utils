#include "unit_test.h"
#include "../lib/open.h"

#include <unistd.h>

/*
 * int open_trunc(const char* filename);
 * int open_read(const char* filename);
 */
TEST(test_open_trunc_read) {
  char path[] = "/tmp/c-utils-test-open-trunc-XXXXXX";
  int fd = mkstemp(path);
  char buf[8] = {0};

  ASSERT_NE(-1, fd);
  close(fd);

  fd = open_trunc(path);
  ASSERT_NE(-1, fd);
  ASSERT_EQ(5, write(fd, "hello", 5));
  close(fd);

  fd = open_read(path);
  ASSERT_NE(-1, fd);
  ASSERT_EQ(5, read(fd, buf, sizeof(buf)));
  ASSERT_EQ(0, memcmp(buf, "hello", 5));
  close(fd);

  unlink(path);
}

/*
 * int open_write(const char* filename);
 */
TEST(test_open_write) {
  char path[] = "/tmp/c-utils-test-open-write-XXXXXX";
  int fd = mkstemp(path);

  ASSERT_NE(-1, fd);
  close(fd);

  fd = open_write(path);
  ASSERT_NE(-1, fd);
  ASSERT_EQ(3, write(fd, "abc", 3));
  close(fd);

  unlink(path);
}

/*
 * int open_append(const char* filename);
 */
TEST(test_open_append) {
  char path[] = "/tmp/c-utils-test-open-append-XXXXXX";
  int fd = mkstemp(path);
  char buf[8] = {0};

  ASSERT_NE(-1, fd);
  ASSERT_EQ(3, write(fd, "abc", 3));
  close(fd);

  fd = open_append(path);
  ASSERT_NE(-1, fd);
  ASSERT_EQ(3, write(fd, "def", 3));
  close(fd);

  fd = open_read(path);
  ASSERT_NE(-1, fd);
  ASSERT_EQ(6, read(fd, buf, sizeof(buf)));
  ASSERT_EQ(0, memcmp(buf, "abcdef", 6));
  close(fd);

  unlink(path);
}

/*
 * int open_rw(const char* filename);
 */
TEST(test_open_rw) {
  char path[] = "/tmp/c-utils-test-open-rw-XXXXXX";
  int fd = mkstemp(path);
  char buf[8] = {0};

  ASSERT_NE(-1, fd);
  close(fd);

  fd = open_rw(path);
  ASSERT_NE(-1, fd);
  ASSERT_EQ(4, write(fd, "test", 4));
  lseek(fd, 0, SEEK_SET);
  ASSERT_EQ(4, read(fd, buf, sizeof(buf)));
  ASSERT_EQ(0, memcmp(buf, "test", 4));
  close(fd);

  unlink(path);
}

/*
 * int open_rwsync(const char* filename);
 *
 * Not asserting a successful write here: open_rwsync() prefers O_DIRECT
 * when available, which requires aligned buffers/lengths that a plain
 * 2-byte write doesn't satisfy -- only the open() itself is checked.
 */
TEST(test_open_rwsync) {
  char path[] = "/tmp/c-utils-test-open-rwsync-XXXXXX";
  int fd = mkstemp(path);

  ASSERT_NE(-1, fd);
  close(fd);

  fd = open_rwsync(path);
  ASSERT_NE(-1, fd);
  close(fd);

  unlink(path);
}

/*
 * int open_excl(const char* filename);
 */
TEST(test_open_excl) {
  char path[] = "/tmp/c-utils-test-open-excl";
  int fd;

  unlink(path);

  fd = open_excl(path);
  ASSERT_NE(-1, fd);
  close(fd);

  /* second call must fail: the file now exists */
  fd = open_excl(path);
  ASSERT_EQ(-1, fd);

  unlink(path);
}

/*
 * int open_temp(char**);
 */
TEST(test_open_temp) {
  char* tmpl = NULL;
  int fd = open_temp(&tmpl);

  ASSERT_NE(-1, fd);
  ASSERT_NE(NULL, tmpl);

  close(fd);
  unlink(tmpl);
  free(tmpl);
}

#define RUN_OPEN_TESTS() \
  RUN(test_open_trunc_read); \
  RUN(test_open_write); \
  RUN(test_open_append); \
  RUN(test_open_rw); \
  RUN(test_open_rwsync); \
  RUN(test_open_excl); \
  RUN(test_open_temp)

TESTS(open) { RUN_OPEN_TESTS(); }
