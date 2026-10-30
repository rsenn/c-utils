#include "unit_test.h"
#include "../lib/binfmt.h"
#include "../lib/mmap.h"

/*
 * int binfmt_open(binfmt_file*, const char* filename);
 */
TEST(test_binfmt_open) {
  binfmt_file bf;

#ifdef __linux__
  /* maps the running executable, which starts with the ELF magic */
  ASSERT_EQ(0, binfmt_open(&bf, "/proc/self/exe"));
  ASSERT_NE(NULL, bf.baseaddr);
  ASSERT_NE(0, bf.filesize);
  ASSERT_EQ(0x7f, bf.baseaddr[0]);
  ASSERT_EQ('E', bf.baseaddr[1]);
  mmap_unmap((const char*)bf.baseaddr, bf.filesize);
#endif
  binfmt_open(&bf, "test_binfmt.nonexistent");
  ASSERT_EQ(NULL, bf.baseaddr);
}

#define RUN_BINFMT_TESTS() RUN(test_binfmt_open)

TESTS(binfmt) { RUN_BINFMT_TESTS(); }
