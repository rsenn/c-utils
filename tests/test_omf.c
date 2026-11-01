#include "unit_test.h"
#include "../lib/omf.h"
#include "../lib/open.h"
#include "../lib/byte.h"
#include "../lib/str.h"
#include <string.h>
#include <unistd.h>

#define OBJFILE "/tmp/test_omf_unit.obj"

static unsigned char buf[512];
static size_t used;

/* appends an OMF record: type, 16 bit length (data + checksum), data, checksum */
static void
rec(unsigned char type, const unsigned char* d, size_t len) {
  unsigned char sum = (unsigned char)(type + (len + 1) % 256 + (len + 1) / 256);
  size_t i;

  for(i = 0; i < len; i++)
    sum = (unsigned char)(sum + d[i]);
  buf[used++] = type;
  buf[used++] = (unsigned char)((len + 1) & 255);
  buf[used++] = (unsigned char)((len + 1) >> 8);
  memcpy(buf + used, d, len);
  used += len;
  buf[used++] = (unsigned char)(0 - sum);
}

/* THEADR "test.c", LNAMES "foo" "bar", PUBDEF "sym", MODEND; written to a file */
static int
make_file(void) {
  unsigned char d[64];
  int k = 0, fd;

  used = 0;
  d[0] = 6;
  memcpy(d + 1, "test.c", 6);
  rec(OMF_THEADR, d, 7);
  d[k++] = 3;
  memcpy(d + k, "foo", 3);
  k += 3;
  d[k++] = 3;
  memcpy(d + k, "bar", 3);
  k += 3;
  rec(OMF_LNAMES, d, (size_t)k);
  k = 0;
  d[k++] = 0; /* group index */
  d[k++] = 1; /* segment index */
  d[k++] = 3;
  memcpy(d + k, "sym", 3);
  k += 3;
  d[k++] = 0x10; /* offset */
  d[k++] = 0;
  d[k++] = 0; /* type index */
  rec(OMF_PUBDEF, d, (size_t)k);
  d[0] = 0;
  rec(OMF_MODEND, d, 1);
  fd = open_trunc(OBJFILE);
  if(fd == -1)
    return 0;
  if(write(fd, buf, used) != (ssize_t)used) {
    close(fd);
    return 0;
  }
  close(fd);
  return 1;
}

/*
 * int omf_open(omf_file*, const char*); omf_begin, omf_end, omf_next, omf_data, omf_name,
 * void omf_close(omf_file*)
 */
TEST(test_omf_records) {
  omf_file f;
  omf_record* r;
  static const unsigned char types[] = {OMF_THEADR, OMF_LNAMES, OMF_PUBDEF, OMF_MODEND};
  static const char* const names[] = {"THEADR", "LNAMES", "PUBDEF", "MODEND"};
  size_t i = 0;
  range d;

  ASSERT_EQ(1, make_file());
  ASSERT_EQ(1, omf_open(&f, OBJFILE));
  ASSERT_EQ(used, f.size);
  ASSERT_EQ(1, omf_begin(&f) == (omf_record*)f.map);
  ASSERT_EQ(1, (unsigned char*)omf_end(&f) - (unsigned char*)omf_begin(&f) == (ptrdiff_t)used);

  omf_foreach(&f, r) {
    ASSERT_EQ(types[i], r->type);
    ASSERT_STR_EQUAL(names[i], omf_name(r));
    ++i;
  }
  ASSERT_EQ(4, i);

  r = omf_begin(&f);
  d = omf_data(r); /* THEADR: length byte, "test.c", checksum */
  ASSERT_EQ(8, (char*)d.end - (char*)d.start);
  ASSERT_EQ(6, (unsigned char)d.start[0]);
  ASSERT_EQ(0, byte_diff(d.start + 1, 6, "test.c"));

  omf_close(&f);
  ASSERT_EQ(1, f.map == NULL);
  unlink(OBJFILE);
  ASSERT_EQ(0, omf_open(&f, "/this/path/does/not/exist.obj"));
}

TEST(test_omf_names_and_iteration) {
  omf_file f;
  omf_record* r;
  range n;
  void* p;
  int count = 0;
  static const char* const lnames[] = {"foo", "bar"};

  ASSERT_EQ(1, make_file());
  ASSERT_EQ(1, omf_open(&f, OBJFILE));
  r = omf_next(omf_begin(&f)); /* LNAMES */
  ASSERT_EQ(OMF_LNAMES, r->type);
  n = omf_get_name(r, 1);
  ASSERT_EQ(3, (char*)n.end - (char*)n.start);
  ASSERT_EQ(0, byte_diff(n.start, 3, "bar"));
  n = omf_get_name(r, 0);
  ASSERT_EQ(0, byte_diff(n.start, 3, "foo"));
  n = omf_get_name(omf_begin(&f), 0); /* not an LNAMES record: empty */
  ASSERT_EQ(1, n.start == n.end);

  omf_record_foreach(r, p) {
    ASSERT_EQ(3, omf_str_len(p));
    ASSERT_EQ(0, byte_diff(omf_str_begin(p), 3, lnames[count]));
    ++count;
  }
  ASSERT_EQ(2, count); /* the checksum byte is not a third name */

  r = omf_next(omf_next(omf_begin(&f))); /* PUBDEF */
  count = 0;
  omf_record_foreach(r, p) {
    ASSERT_EQ(3, omf_str_len(p));
    ASSERT_EQ(0, byte_diff(omf_str_begin(p), 3, "sym"));
    ++count;
  }
  ASSERT_EQ(1, count);

  /* a record kind that cannot be walked is an empty iteration, not an endless one */
  r = omf_begin(&f); /* THEADR */
  count = 0;
  omf_record_foreach(r, p) {
    if(++count > 3)
      break;
  }
  ASSERT_EQ(0, count);
  omf_close(&f);
  unlink(OBJFILE);
}

#define RUN_OMF_TESTS() \
  RUN(test_omf_records); \
  RUN(test_omf_names_and_iteration)

TESTS(omf) { RUN_OMF_TESTS(); }
