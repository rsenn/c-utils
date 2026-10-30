#include "unit_test.h"
#include "../lib/ihex.h"
#include "../lib/buffer.h"
#include "../lib/open.h"
#include "../lib/byte.h"
#include "../lib/str.h"
#include <unistd.h>

/* two data records, an extended linear address and the end of file */
static const char text[] = ":020000040001F9\r\n"
                           ":0400100001020304E2\r\n"
                           ":02002000AABB79\r\n"
                           "this line is no record\r\n"
                           ":00000001FF\r\n";

/*
 * ssize_t ihex_read_buf(ihex_file*, const char*, size_t);
 */
TEST(test_ihex_read_buf) {
  ihex_file f;

  ihex_init(&f);

  /* the line that is no record is skipped */
  ASSERT_EQ(4, ihex_read_buf(&f, text, sizeof(text) - 1));
  ASSERT_EQ(0, ihex_verify(&f));
}

/*
 * size_t ihex_verify(ihex_file*);
 */
TEST(test_ihex_verify) {
  static const char bad[] = ":0400100001020304E3\n:00000001FF\n";
  ihex_file f;

  ihex_init(&f);
  ihex_read_buf(&f, bad, sizeof(bad) - 1);

  ASSERT_EQ(1, ihex_verify(&f));
}

/*
 * size_t ihex_range(ihex_file*, uint32* lo, uint32* hi);
 */
TEST(test_ihex_range) {
  ihex_file f;
  uint32 lo = 0, hi = 0;

  ihex_init(&f);
  ihex_read_buf(&f, text, sizeof(text) - 1);

  ASSERT_EQ(6, ihex_range(&f, &lo, &hi));
  ASSERT_EQ(0x10010, lo);
  ASSERT_EQ(0x10022, hi);
}

/*
 * size_t ihex_copy_to(ihex_file*, uint32 base, uint8* image, uint8* map, size_t size);
 */
TEST(test_ihex_copy_to) {
  ihex_file f;
  uint8 image[0x20], map[0x20];

  ihex_init(&f);
  ihex_read_buf(&f, text, sizeof(text) - 1);
  byte_fill(image, sizeof(image), 0xff);
  byte_zero(map, sizeof(map));

  ASSERT_EQ(6, ihex_copy_to(&f, 0x10010, image, map, sizeof(image)));
  ASSERT_EQ(1, image[0]);
  ASSERT_EQ(4, image[3]);
  ASSERT_EQ(0xff, image[4]);
  ASSERT_EQ(0, map[4]);
  ASSERT_EQ(0xaa, image[0x10]);
  ASSERT_EQ(1, map[0x11]);
}

/*
 * void ihex_write_data(buffer*, uint16* hi16, uint32 addr, const uint8* x, size_t n, unsigned reclen);
 * void ihex_write_eof(buffer*);
 */
TEST(test_ihex_write_data) {
  uint8 data[40], image[0x100], map[0x100];
  uint16 hi16 = 0;
  buffer out, in;
  ihex_file f;
  char buf[2048];
  ssize_t n;
  size_t i;

  for(i = 0; i < sizeof(data); i++)
    data[i] = (uint8)(i * 3);

  buffer_write_fd(&out, open_trunc("test_ihex.tmp"));
  /* crosses the 64k boundary at 0xfff0 */
  ihex_write_data(&out, &hi16, 0xffe0, data, sizeof(data), 16);
  ihex_write_eof(&out);
  buffer_flush(&out);
  buffer_close(&out);

  ASSERT_EQ(0, buffer_mmapprivate(&in, "test_ihex.tmp"));
  n = buffer_get(&in, buf, sizeof(buf));
  buffer_close(&in);
  unlink("test_ihex.tmp");

  ASSERT_GT(n, 0);
  ASSERT_EQ(':', buf[0]);

  ihex_init(&f);
  ASSERT_EQ(0, ihex_verify(&f));
  ihex_read_buf(&f, buf, (size_t)n);
  ASSERT_EQ(0, ihex_verify(&f));

  byte_zero(map, sizeof(map));
  ASSERT_EQ(40, ihex_copy_to(&f, 0xffe0, image, map, sizeof(image)));
  ASSERT_EQ(0, byte_diff(image, 40, data));
  /* an extended linear address record 1 appears for the part past 0xffff */
  ASSERT_EQ(1, hi16);
}

#define RUN_IHEX_TESTS() \
  RUN(test_ihex_read_buf); \
  RUN(test_ihex_verify); \
  RUN(test_ihex_range); \
  RUN(test_ihex_copy_to); \
  RUN(test_ihex_write_data)

TESTS(ihex) { RUN_IHEX_TESTS(); }
