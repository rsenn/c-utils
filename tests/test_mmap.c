#include "unit_test.h"
#include "../lib/mmap.h"

#include <fcntl.h>
#include <unistd.h>

/*
 * char* mmap_read(const char* filename, size_t* filesize);
 * int mmap_unmap(const char* mapped, size_t maplen);
 */
TEST(test_mmap_read) {
  char path[] = "/tmp/c-utils-test-mmap-read-XXXXXX";
  int fd = mkstemp(path);
  size_t sz = 0;
  char* map;

  ASSERT_NE(-1, fd);
  ASSERT_EQ(5, write(fd, "hello", 5));
  close(fd);

  map = mmap_read(path, &sz);

  ASSERT_NE(NULL, map);
  ASSERT_EQ(5, sz);
  ASSERT_EQ(0, memcmp(map, "hello", 5));

  mmap_unmap(map, sz);
  unlink(path);
}

/*
 * char* mmap_read_fd(fd_type fd, size_t* filesize);
 */
TEST(test_mmap_read_fd) {
  char path[] = "/tmp/c-utils-test-mmap-readfd-XXXXXX";
  int fd = mkstemp(path);
  size_t sz = 0;
  char* map;

  ASSERT_NE(-1, fd);
  ASSERT_EQ(3, write(fd, "abc", 3));

  map = mmap_read_fd(fd, &sz);

  ASSERT_NE(NULL, map);
  ASSERT_EQ(3, sz);
  ASSERT_EQ(0, memcmp(map, "abc", 3));

  mmap_unmap(map, sz);
  close(fd);
  unlink(path);
}

/*
 * char* mmap_read_fd_range(fd_type fd, size_t* size, size_t offset, size_t len);
 *
 * offset is passed straight through to mmap(2), so it must be page
 * aligned like a raw mmap() offset would be; only offset 0 is exercised
 * here.
 */
TEST(test_mmap_read_fd_range) {
  char path[] = "/tmp/c-utils-test-mmap-range-XXXXXX";
  int fd = mkstemp(path);
  size_t sz = 0;
  char* map;

  ASSERT_NE(-1, fd);
  ASSERT_EQ(10, write(fd, "0123456789", 10));

  map = mmap_read_fd_range(fd, &sz, 0, 4);

  ASSERT_NE(NULL, map);
  ASSERT_EQ(4, sz);
  ASSERT_EQ(0, memcmp(map, "0123", 4));

  mmap_unmap(map, sz);
  close(fd);
  unlink(path);
}

/*
 * char* mmap_map(fd_type fd, size_t sz, uint64 offset);
 */
TEST(test_mmap_map) {
  char path[] = "/tmp/c-utils-test-mmap-map-XXXXXX";
  int fd = mkstemp(path);
  char* map;

  ASSERT_NE(-1, fd);
  ASSERT_EQ(4, write(fd, "test", 4));

  map = mmap_map(fd, 4, 0);

  ASSERT_NE(NULL, map);
  ASSERT_EQ(0, memcmp(map, "test", 4));

  mmap_unmap(map, 4);
  close(fd);
  unlink(path);
}

/*
 * char* mmap_private(const char* filename, size_t* filesize);
 * char* mmap_private_fd(fd_type fd, size_t* filesize);
 */
TEST(test_mmap_private) {
  char path[] = "/tmp/c-utils-test-mmap-private-XXXXXX";
  int fd = mkstemp(path);
  size_t sz = 0;
  char* map;

  ASSERT_NE(-1, fd);
  ASSERT_EQ(5, write(fd, "abcde", 5));
  close(fd);

  map = mmap_private(path, &sz);

  ASSERT_NE(NULL, map);
  ASSERT_EQ(5, sz);
  ASSERT_EQ(0, memcmp(map, "abcde", 5));

  /* private mappings are copy-on-write: mutating must not touch the file */
  map[0] = 'X';

  mmap_unmap(map, sz);
  unlink(path);
}

/*
 * char* mmap_shared(const char* filename, size_t* filesize);
 * char* mmap_shared_fd(fd_type fd, size_t* filesize);
 */
TEST(test_mmap_shared) {
  char path[] = "/tmp/c-utils-test-mmap-shared-XXXXXX";
  int fd = mkstemp(path);
  size_t sz = 0;
  char* map;

  ASSERT_NE(-1, fd);
  ASSERT_EQ(5, write(fd, "abcde", 5));
  close(fd);

  map = mmap_shared(path, &sz);

  ASSERT_NE(NULL, map);
  ASSERT_EQ(5, sz);

  map[0] = 'X';
  mmap_unmap(map, sz);

  fd = open(path, O_RDONLY);
  char buf[5];
  ASSERT_EQ(5, read(fd, buf, 5));
  ASSERT_EQ('X', buf[0]);
  close(fd);

  unlink(path);
}

/*
 * char* mmap_anon(size_t size);
 *
 * Not tested: declared in mmap.h but has no implementation anywhere in
 * the tree (would fail to link). See BUGS.
 */
TEST(test_mmap_anon) {}

/*
 * char* mmap_rw_fd(fd_type, size_t*);
 */
TEST(test_mmap_rw_fd) {
  char path[] = "/tmp/c-utils-test-mmap-rw-XXXXXX";
  int fd = mkstemp(path);
  size_t sz = 0;
  char* map;

  ASSERT_NE(-1, fd);
  ASSERT_EQ(4, write(fd, "data", 4));

  map = mmap_rw_fd(fd, &sz);

  ASSERT_NE(NULL, map);
  ASSERT_EQ(4, sz);
  ASSERT_EQ(0, memcmp(map, "data", 4));

  mmap_unmap(map, sz);
  close(fd);
  unlink(path);
}

#define RUN_MMAP_TESTS() \
  RUN(test_mmap_read); \
  RUN(test_mmap_read_fd); \
  RUN(test_mmap_read_fd_range); \
  RUN(test_mmap_map); \
  RUN(test_mmap_private); \
  RUN(test_mmap_shared); \
  RUN(test_mmap_anon); \
  RUN(test_mmap_rw_fd)

TESTS(mmap) { RUN_MMAP_TESTS(); }
