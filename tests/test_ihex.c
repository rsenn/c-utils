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

static ssize_t
discard(int fd, const void* x, size_t n, void* b) {
  (void)fd;
  (void)x;
  (void)b;
  return (ssize_t)n;
}

/*
 * ihex_record* ihex_put(ihex_file*, uint32 offset, const char*, size_t);
 * size_t ihex_read_at(ihex_file*, uint32 at, char*, size_t);
 * uint8 ihex_record_checksum(const ihex_record*);
 */
TEST(test_ihex_put_read_at) {
  ihex_file f;
  ihex_record* r;
  char out[8];

  ihex_init(&f);
  r = ihex_put(&f, 0x100, "abcd", 4);
  ASSERT_EQ(1, r != NULL);
  ASSERT_EQ(4, r->length);
  ASSERT_EQ(0x100, r->offset);
  ASSERT_EQ(IHEX_DATA, r->type);
  ASSERT_EQ(0x71, r->checksum);
  ASSERT_EQ(r->checksum, ihex_record_checksum(r));
  ASSERT_EQ(1, ihex_put(&f, 0x10020, "XY", 2) != NULL); /* beyond 64k: an address record is added */

  ASSERT_EQ(4, ihex_read_at(&f, 0x100, out, 4));
  ASSERT_EQ(0, byte_diff(out, 4, "abcd"));
  ASSERT_EQ(2, ihex_read_at(&f, 0x10020, out, 2));
  ASSERT_EQ(0, byte_diff(out, 2, "XY"));
  ASSERT_EQ(0, ihex_read_at(&f, 0x5000, out, 2)); /* nothing there */
  ASSERT_EQ(0, ihex_verify(&f));
}

/*
 * ihex_record* ihex_record_at / ihex_record_find / ihex_record_insert / next / prev
 * int ihex_record_address / ihex_record_base
 */
TEST(test_ihex_records) {
  ihex_file f;
  ihex_record *r, *q, *d;
  ihex_recptr p;
  uint32 roff = 0, base = 0;
  ihex_addr a;

  ihex_init(&f);
  ihex_put(&f, 0x100, "abcd", 4);
  ihex_put(&f, 0x10020, "XY", 2);

  r = ihex_record_at(&f, 0x102, &roff);
  ASSERT_EQ(1, r != NULL);
  ASSERT_EQ(0x100, roff); /* *roffs is the address at which the record starts */
  ASSERT_EQ(0x100, r->offset);
  p = ihex_record_find(&f, 0x102, &roff);
  ASSERT_EQ(1, p.rec == r);

  q = ihex_record_insert(&f, 0x200, 3);
  ASSERT_EQ(1, q != NULL);
  ASSERT_EQ(3, q->length);
  ASSERT_EQ(0x200, q->offset);
  ASSERT_EQ(0, ihex_record_insert(&f, 0x101, 2) != NULL); /* overlaps the first record */

  d = ihex_record_next(&f, (ihex_record*)f.records.next, IHEX_DATA);
  ASSERT_EQ(1, d == q); /* the record after the first data record */
  ASSERT_EQ(1, ihex_record_prev(&f, d, IHEX_DATA) == r);

  ASSERT_EQ(IHEX_DATA, ihex_record_address(r, &a));
  ASSERT_EQ(0x100, a.lo16);
  /* ihex_record_base: nonzero for the records that set the base address */
  ASSERT_EQ(0, ihex_record_base(r, &base));
  ASSERT_EQ(1, ihex_record_base((ihex_record*)q->link.next, &base)); /* the extended address record */
  ASSERT_EQ(0x10000, base);
}

/*
 * ssize_t ihex_record_read(ihex_record**, const char*, size_t);
 * void ihex_record_write(ihex_record*, buffer*); void ihex_record_dump(...); int ihex_write(...)
 */
TEST(test_ihex_record_io) {
  ihex_record* r = NULL;
  ihex_file f;
  buffer b;
  char mem[256];
  ssize_t n;

  n = ihex_record_read(&r, ":0400000100020304F6\n", 19);
  ASSERT_EQ(1, n > 0);
  ASSERT_EQ(4, r->length);
  ASSERT_EQ(IHEX_EOF, r->type);                                  /* type 01 */
  ASSERT_EQ(0, ihex_record_read(&r, ":04000001000203FF\n", 18)); /* bad checksum: refused */

  ihex_init(&f);
  ihex_put(&f, 0x300, "Q", 1);
  buffer_init(&b, (buffer_op_ptr)discard, -1, mem, sizeof mem);
  ASSERT_EQ(1, ihex_write(&f, &b) >= 0); /* the discarding writer swallows it */
  ihex_record_dump(r, 0, &b);
  buffer_flush(&b);
}

/* a record made by ihex_record_insert and filled afterwards is written with a correct checksum */
TEST(test_ihex_insert_then_write) {
  ihex_file f, g;
  ihex_record* r;
  buffer out;
  char mem[512];
  char text[512];
  int fd;
  ssize_t n;

  ihex_init(&f);
  r = ihex_record_insert(&f, 0x40, 3);
  r->data[0] = 1;
  r->data[1] = 2;
  r->data[2] = 3;
  fd = open_trunc("test_ihex2.tmp");
  buffer_init(&out, (buffer_op_ptr)write, fd, mem, sizeof mem);
  ihex_record_write(r, &out);
  buffer_flush(&out);
  close(fd);
  fd = open_read("test_ihex2.tmp");
  n = read(fd, text, sizeof text);
  close(fd);
  unlink("test_ihex2.tmp");
  ASSERT_EQ(1, n > 0);
  ihex_init(&g);
  ASSERT_EQ(1, ihex_read_buf(&g, text, (size_t)n));
  ASSERT_EQ(0, ihex_verify(&g));
}

/*
 * size_t ihex_strip_below(ihex_file*, uint32 addr): the bootloader build
 */
TEST(test_ihex_strip_below) {
  ihex_file f;
  char out[8];
  uint32 lo = 0, hi = 0;

  ihex_init(&f);
  ihex_put(&f, 0x0000, "RST!", 4);   /* the reset stub of a bootloaded program */
  ihex_put(&f, 0x1ffe, "abcdef", 6); /* crosses the load address 0x2000 */
  ihex_put(&f, 0x2100, "tail", 4);

  ASSERT_EQ(4 + 2, ihex_strip_below(&f, 0x2000)); /* the stub, and the two bytes before the address */
  ASSERT_EQ(0, ihex_read_at(&f, 0x0, out, 4));
  ASSERT_EQ(0, ihex_read_at(&f, 0x1ffe, out, 2));
  ASSERT_EQ(4, ihex_read_at(&f, 0x2000, out, 4));
  ASSERT_EQ(0, byte_diff(out, 4, "cdef"));
  ASSERT_EQ(4, ihex_read_at(&f, 0x2100, out, 4));
  ASSERT_EQ(8, ihex_range(&f, &lo, &hi));
  ASSERT_EQ(0x2000, lo);
  ASSERT_EQ(0, ihex_verify(&f));              /* checksums of the trimmed record are right */
  ASSERT_EQ(0, ihex_strip_below(&f, 0x2000)); /* idempotent */
}

#define RUN_IHEX_TESTS() \
  RUN(test_ihex_put_read_at); \
  RUN(test_ihex_records); \
  RUN(test_ihex_record_io); \
  RUN(test_ihex_insert_then_write); \
  RUN(test_ihex_strip_below); \
  RUN(test_ihex_read_buf); \
  RUN(test_ihex_verify); \
  RUN(test_ihex_range); \
  RUN(test_ihex_copy_to); \
  RUN(test_ihex_write_data)

TESTS(ihex) { RUN_IHEX_TESTS(); }
