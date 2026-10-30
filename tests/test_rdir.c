#include "unit_test.h"
#include "../lib/rdir.h"
#include "../lib/str.h"
#include "../lib/open.h"
#include <unistd.h>
#include <sys/stat.h>

static int
touch(const char* path) {
  int fd = open_trunc(path);

  if(fd < 0)
    return -1;
  close(fd);
  return 0;
}

/*
 * int rdir_open(rdir_t*, const char*);
 * char* rdir_read(rdir_t*);
 * void rdir_close(rdir_t*);
 */
TEST(test_rdir_read) {
  rdir_t rd;
  char* s;
  unsigned found = 0;

  mkdir("test_rdir.tmp", 0755);
  mkdir("test_rdir.tmp/sub", 0755);
  touch("test_rdir.tmp/a.txt");
  touch("test_rdir.tmp/sub/b.txt");

  ASSERT_EQ(0, rdir_open(&rd, "test_rdir.tmp"));

  while((s = rdir_read(&rd))) {
    if(str_end(s, "a.txt"))
      found |= 1;
    else if(str_end(s, "sub/b.txt"))
      found |= 2;
  }
  rdir_close(&rd);

  unlink("test_rdir.tmp/sub/b.txt");
  unlink("test_rdir.tmp/a.txt");
  rmdir("test_rdir.tmp/sub");
  rmdir("test_rdir.tmp");

  /* both the file in the directory and the one in its subdirectory */
  ASSERT_EQ(3, found);
}

#define RUN_RDIR_TESTS() RUN(test_rdir_read)

TESTS(rdir) { RUN_RDIR_TESTS(); }
