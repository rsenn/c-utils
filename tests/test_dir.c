#include "unit_test.h"
#include "../lib/stralloc.h"
#include "../lib/str.h"
#include "../lib/dir.h"
#include "../lib/unix.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * int dir_open(struct dir_s* d, const char* p);
 * char* dir_read(struct dir_s* d);
 * void dir_close(struct dir_s* d);
 */
TEST(test_dir_open_read_close) {
  struct dir_s d;
  char path[] = "/tmp/c-utils-test-dir-XXXXXX";
  char file[64];
  char* name;
  int found = 0;
  int fd;

  ASSERT_NE(NULL, mkdtemp(path));

  str_copy(file, path);
  str_cat(file, "/entry.txt");

  fd = open(file, O_CREAT | O_WRONLY, 0644);
  ASSERT_NE(-1, fd);
  close(fd);

  ASSERT_EQ(0, dir_open(&d, path));

  while((name = dir_read(&d))) {
    if(!str_diff(name, "entry.txt"))
      found = 1;
  }

  dir_close(&d);

  ASSERT_NE(0, found);

  unlink(file);
  rmdir(path);
}

/*
 * char* dir_name(struct dir_s*);
 * int dir_type(struct dir_s*);
 * int64 dir_size(struct dir_s*);
 *
 * dir_size() is only implemented for the Windows FindFirstFile backend;
 * on POSIX (opendir/readdir) it unconditionally returns -1. See BUGS
 * (dir-size-windows-only).
 */
TEST(test_dir_name_type_size) {
  struct dir_s d;
  char path[] = "/tmp/c-utils-test-dir-meta-XXXXXX";
  char file[64];
  char* name;
  int fd;

  ASSERT_NE(NULL, mkdtemp(path));

  str_copy(file, path);
  str_cat(file, "/data.bin");

  fd = open(file, O_CREAT | O_WRONLY, 0644);
  ASSERT_NE(-1, fd);
  ASSERT_EQ(4, write(fd, "abcd", 4));
  close(fd);

  ASSERT_EQ(0, dir_open(&d, path));

  while((name = dir_read(&d))) {
    if(!str_diff(name, "data.bin")) {
      ASSERT_EQ(0, str_diff(dir_name(&d), "data.bin"));
      ASSERT_EQ(D_FILE, dir_type(&d));
      ASSERT_EQ(-1, dir_size(&d));
    }
  }

  dir_close(&d);

  unlink(file);
  rmdir(path);
}

/*
 * unsigned long dir_time(struct dir_s*, int time_type);
 */
TEST(test_dir_time) {
  struct dir_s d;
  char path[] = "/tmp/c-utils-test-dir-time-XXXXXX";
  char file[64];
  char* name;
  int fd;

  ASSERT_NE(NULL, mkdtemp(path));

  str_copy(file, path);
  str_cat(file, "/f");

  fd = open(file, O_CREAT | O_WRONLY, 0644);
  ASSERT_NE(-1, fd);
  close(fd);

  ASSERT_EQ(0, dir_open(&d, path));

  while((name = dir_read(&d))) {
    if(!str_diff(name, "f")) {
      ASSERT_NE(0, dir_time(&d, D_TIME_MODIFICATION));
    }
  }

  dir_close(&d);

  unlink(file);
  rmdir(path);
}

/*
 * void dir_path(struct dir_s* d, stralloc* sa);
 *
 * Builds the full path (directory + entry name) of the current
 * directory entry into `sa`.
 */
TEST(test_dir_path) {
  struct dir_s d;
  char path[] = "/tmp/c-utils-test-dir-path-XXXXXX";
  char file[64];
  char expect[64];
  char* name;
  int fd;
  stralloc sa;

  stralloc_init(&sa);

  ASSERT_NE(NULL, mkdtemp(path));

  str_copy(file, path);
  str_cat(file, "/f");

  str_copy(expect, path);
  str_cat(expect, "/f");

  fd = open(file, O_CREAT | O_WRONLY, 0644);
  ASSERT_NE(-1, fd);
  close(fd);

  ASSERT_EQ(0, dir_open(&d, path));

  while((name = dir_read(&d))) {
    if(!str_diff(name, "f")) {
      dir_path(&d, &sa);
      ASSERT_EQ(0, stralloc_diffs(&sa, expect));
    }
  }

  dir_close(&d);
  stralloc_free(&sa);

  unlink(file);
  rmdir(path);
}

#define RUN_DIR_TESTS() \
  RUN(test_dir_open_read_close); \
  RUN(test_dir_name_type_size); \
  RUN(test_dir_time); \
  RUN(test_dir_path)

TESTS(dir) { RUN_DIR_TESTS(); }
