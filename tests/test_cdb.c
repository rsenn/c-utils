#include "unit_test.h"
#include "../lib/cdb.h"
#include "../lib/cdb_make.h"
#include "../lib/open.h"
#include "../lib/str.h"
#include "../lib/byte.h"
#include <unistd.h>

#define CDBFILE "/tmp/test_cdb_unit.cdb"

TEST(test_cdb_roundtrip) {
  struct cdb_make cm;
  struct cdb c = {0};
  int64 fd;
  uint32 pos;
  int n;
  char k[8];

  fd = open_trunc(CDBFILE);
  ASSERT_NE(-1, (long)fd);
  ASSERT_EQ(2048, cdb_make_start(&cm, fd)); /* libowfat: returns the lseek offset */
  ASSERT_EQ(0, cdb_make_add(&cm, (const unsigned char*)"one", 3, (const unsigned char*)"1", 1));
  ASSERT_EQ(0, cdb_make_add(&cm, (const unsigned char*)"two", 3, (const unsigned char*)"22", 2));
  ASSERT_EQ(0, cdb_make_add(&cm, (const unsigned char*)"two", 3, (const unsigned char*)"222", 3));
  ASSERT_EQ(0, cdb_make_finish(&cm));
  close((int)fd);

  fd = open_read(CDBFILE);
  ASSERT_NE(-1, (long)fd);
  cdb_init(&c, fd);

  ASSERT_EQ(1, cdb_find(&c, (const unsigned char*)"one", 3));
  ASSERT_EQ(1, c.dlen);
  ASSERT_EQ(0, cdb_find(&c, (const unsigned char*)"three", 5));

  /* two entries for "two": findnext iterates */
  cdb_findstart(&c);
  n = 0;
  while(cdb_findnext(&c, (const unsigned char*)"two", 3) > 0)
    ++n;
  ASSERT_EQ(2, n);

  /* traverse all keys */
  n = 0;
  if(cdb_firstkey(&c, &pos) > 0) {
    do
      ++n;
    while(cdb_nextkey(&c, &pos) > 0);
  }
  ASSERT_EQ(3, n);
  (void)k;
  cdb_free(&c);
  close((int)fd);
  unlink(CDBFILE);
}

TEST(test_cdb_hash) {
  /* djb's cdb hash: start 5381, h = (h*33) ^ c */
  ASSERT_EQ(5381u, cdb_hash((const unsigned char*)"", 0));
  ASSERT_EQ((5381u * 33) ^ 'a', cdb_hash((const unsigned char*)"a", 1));
}

#define RUN_CDB_TESTS() \
  RUN(test_cdb_roundtrip); \
  RUN(test_cdb_hash)

TESTS(cdb) { RUN_CDB_TESTS(); }
