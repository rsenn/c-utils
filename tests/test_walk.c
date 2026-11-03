#include "unit_test.h"
#include "../lib/walk.h"
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
 * int walk_open(struct walk*, const char*, unsigned);
 * struct walk_ent* walk_read(struct walk*);
 * void walk_close(struct walk*);
 */
TEST(test_walk_read) {
  struct walk w;
  struct walk_ent* e;
  unsigned found = 0;

  mkdir("test_walk.tmp", 0755);
  mkdir("test_walk.tmp/sub", 0755);
  touch("test_walk.tmp/a.txt");
  touch("test_walk.tmp/sub/b.txt");

  ASSERT_EQ(0, walk_open(&w, "test_walk.tmp", 0));

  while((e = walk_read(&w))) {
    if(str_end(e->path, "a.txt"))
      found |= 1;
    else if(str_end(e->path, "sub/b.txt"))
      found |= 2;
  }
  walk_close(&w);

  unlink("test_walk.tmp/sub/b.txt");
  unlink("test_walk.tmp/a.txt");
  rmdir("test_walk.tmp/sub");
  rmdir("test_walk.tmp");

  /* both the file in the directory and the one in its subdirectory */
  ASSERT_EQ(3, found);
}

#define RUN_WALK_TESTS() RUN(test_walk_read)

TESTS(walk) { RUN_WALK_TESTS(); }
