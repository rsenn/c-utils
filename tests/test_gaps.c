#include "../lib/stralloc.h"
#include "../lib/array.h"
#include "unit_test.h"
#include "../lib/textcode.h"
#include "../lib/rangecheck.h"
#include "../lib/wait.h"
#include "../lib/charbuf.h"
#include "../lib/hashmap.h"
#include "../lib/mmap.h"
#include "../lib/open.h"
#include "../lib/byte.h"
#include "../lib/str.h"
#include "../lib/buffer.h"
#include "../lib/uint16.h"
#include "../lib/uint32.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>

/*
 * int fmt_to_sa(fmt, sa, src, len); size_t scan_to_sa(scan, src, sa);
 * void fmt_to_array(fmt, array*, src, len); size_t scan_to_array(scan, src, array*);
 * void fmt_tofrom_array(fmt, dest, src); size_t scan_tofrom_array(scan, src, dest);
 */
TEST(test_textcode_sa_array) {
  stralloc sa;
  array a, b, c;

  stralloc_init(&sa);
  ASSERT_EQ(1, fmt_to_sa(fmt_base64, &sa, "foobar", 6) > 0);
  ASSERT_EQ(8, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s, 8, "Zm9vYmFy"));

  stralloc_zero(&sa);
  stralloc_nul(&sa);
  ASSERT_EQ(8, scan_to_sa(scan_base64, "Zm9vYmFy", &sa));
  ASSERT_EQ(6, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s, 6, "foobar"));
  stralloc_free(&sa);

  byte_zero(&a, sizeof a);
  byte_zero(&b, sizeof b);
  byte_zero(&c, sizeof c);
  fmt_to_array(fmt_hexdump, &a, "\x01\xab", 2);
  ASSERT_EQ(4, array_bytes(&a));
  ASSERT_EQ(0, byte_diff(array_start(&a), 4, "01ab"));

  fmt_tofrom_array(fmt_base64, &b, &a); /* base64 of the text "01ab" */
  ASSERT_EQ(8, array_bytes(&b));
  ASSERT_EQ(0, byte_diff(array_start(&b), 8, "MDFhYg=="));

  ASSERT_EQ(8, scan_to_array(scan_base64, "MDFhYg==", &c));
  ASSERT_EQ(4, array_bytes(&c));
  ASSERT_EQ(0, byte_diff(array_start(&c), 4, "01ab"));

  array_reset(&c);
  byte_zero(&c, sizeof c);
  ASSERT_EQ(8, scan_tofrom_array(scan_base64, &b, &c));
  ASSERT_EQ(4, array_bytes(&c));
  array_reset(&a);
  array_reset(&b);
  array_reset(&c);
}

/*
 * size_t fmt_ldapescape2(char*, const char*, size_t, const char* escapeme);
 * size_t scan_html(const char*, char*, size_t*);
 */
TEST(test_textcode_ldapescape2_scan_html) {
  char out[64];
  size_t n, dl = 0;

  n = fmt_ldapescape2(out, "a*b(c", 5, "*");
  ASSERT_EQ(1, n >= 5);
  ASSERT_EQ('a', out[0]);
  ASSERT_EQ(n, fmt_ldapescape2(NULL, "a*b(c", 5, "*"));

  n = scan_html("a&lt;b&amp;c&gt;", out, &dl);
  ASSERT_EQ(16, n);
  ASSERT_EQ(6, dl);
  ASSERT_EQ(0, byte_diff(out, 6, "a<b&c>"));
}

/*
 * range_arrayinbuf, range_strinbuf, range_str2inbuf, range_str4inbuf
 */
TEST(test_range_inbuf) {
  char buf[16];
  uint16 w[8];
  uint32 l[4];

  byte_zero(buf, sizeof buf);
  byte_copy(buf, 6, "hello");
  ASSERT_EQ(1, range_strinbuf(buf, sizeof buf, buf));
  ASSERT_EQ(1, range_strinbuf(buf, sizeof buf, buf + 3));
  ASSERT_EQ(0, range_strinbuf(buf, sizeof buf, buf + 16)); /* outside */
  memset(buf, 'x', sizeof buf); /* no terminator at all */
  ASSERT_EQ(0, range_strinbuf(buf, sizeof buf, buf));
  buf[10] = 0;
  ASSERT_EQ(1, range_strinbuf(buf, sizeof buf, buf));
  ASSERT_EQ(0, range_strinbuf(buf, 10, buf)); /* the NUL is beyond the buffer */

  memset(w, 0xff, sizeof w);
  ASSERT_EQ(0, range_str2inbuf(w, sizeof w, w));
  w[5] = 0;
  ASSERT_EQ(1, range_str2inbuf(w, sizeof w, w));
  ASSERT_EQ(0, range_str2inbuf(w, 5 * sizeof(uint16), w));

  memset(l, 0xff, sizeof l);
  ASSERT_EQ(0, range_str4inbuf(l, sizeof l, l));
  l[2] = 0;
  ASSERT_EQ(1, range_str4inbuf(l, sizeof l, l));
  ASSERT_EQ(0, range_str4inbuf(l, 2 * sizeof(uint32), l));

  ASSERT_EQ(1, range_arrayinbuf(buf, sizeof buf, buf, 4, 4));
  ASSERT_EQ(1, range_arrayinbuf(buf, sizeof buf, buf + 8, 2, 4));
  ASSERT_EQ(0, range_arrayinbuf(buf, sizeof buf, buf + 8, 3, 4));
  ASSERT_EQ(0, range_arrayinbuf(buf, sizeof buf, buf, (size_t)-1, 4)); /* overflow */
}

/*
 * unsigned int wait_reap(void);
 * int wait_pids_nohang(int const*, unsigned int, int*);
 * int wait_pids_reap(int*, unsigned int);
 */
TEST(test_wait_reap_nohang) {
  int pids[2], i, w = 0, r;

  for(i = 0; i < 2; ++i) {
    pids[i] = fork();
    if(pids[i] == 0)
      _exit(7 + i);
    ASSERT_EQ(1, pids[i] > 0);
  }
  /* poll until the first has been collected */
  for(i = 0, r = 0; i < 1000 && r == 0; ++i) {
    r = wait_pids_nohang(pids, 2, &w);
    if(r == 0)
      usleep(1000);
  }
  ASSERT_EQ(1, r == 1 || r == 2);
  ASSERT_EQ(1, WIFEXITED(w));
  ASSERT_EQ(r == 1 ? 7 : 8, WEXITSTATUS(w));

  /* the other one */
  for(i = 0; i < 1000; ++i) {
    unsigned int n = wait_reap();
    if(n)
      break;
    usleep(1000);
  }
  ASSERT_EQ(0, wait_reap()); /* no children left */
  {
    int one = fork();
    if(one == 0)
      _exit(0);
    for(i = 0; i < 1000; ++i) {
      int pp[1];
      pp[0] = one;
      if(wait_pids_reap(pp, 1) == 1)
        break;
      usleep(1000);
    }
    ASSERT_EQ(1, i < 1000);
  }
}

/*
 * ssize_t charbuf_skip(charbuf*); charbuf_skipc(charbuf*, uint8*);
 */
TEST(test_charbuf_skip_skipc) {
  charbuf cb;
  uint8 c = 0;
  char text[] = "abc";

  charbuf_froms(&cb, text, 4);
  ASSERT_EQ('a', charbuf_skip(&cb));
  ASSERT_EQ(1, charbuf_skipc(&cb, &c));
  ASSERT_EQ('b', c);
  ASSERT_EQ('c', charbuf_get(&cb));
  ASSERT_EQ(1, charbuf_skip(&cb) <= 0); /* end of input */
  charbuf_free(&cb);
}

/*
 * hashmap_init, hashmap_get_entry, hashmap_delete2, hashmap_rehash
 */
TEST(test_hashmap_entry_delete2_rehash) {
  hashmap m;
  hashentry* e;
  int i, v[100];
  char key[16];

  hashmap_init(&m);
  ASSERT_EQ(0, m.used);
  for(i = 0; i < 100; ++i) {
    key[0] = 'k';
    key[1] = (char)('0' + i / 10);
    key[2] = (char)('0' + i % 10);
    v[i] = i;
    hashmap_put2(&m, key, 3, &v[i]);
  }
  ASSERT_EQ(100, m.used);
  hashmap_rehash(&m);
  ASSERT_EQ(100, m.used);
  e = hashmap_get_entry(&m, "k42", 3);
  ASSERT_EQ(1, e != NULL);
  ASSERT_EQ(&v[42], e->val);
  ASSERT_EQ(NULL, hashmap_get_entry(&m, "k99x", 4));
  hashmap_delete2(&m, "k42", 3);
  ASSERT_EQ(NULL, hashmap_get2(&m, "k42", 3));
  ASSERT_EQ(&v[41], hashmap_get2(&m, "k41", 3));
  ASSERT_EQ(100, m.used); /* the deleted bucket is a tombstone and still counts */
  free(m.buckets);
}

/*
 * char* mmap_private_fd(fd, size_t*); char* mmap_shared_fd(fd, size_t*);
 * const char* mmap_readat(filename, size_t*, dirfd); ssize_t fd_filesize(fd);
 */
TEST(test_mmap_fd_variants) {
  const char* fn = "/tmp/c-utils-mmapfd.tmp";
  int fd = open_trunc(fn);
  size_t n = 0;
  char* p;

  write(fd, "0123456789", 10);
  close(fd);

  fd = open_rw(fn);
  ASSERT_EQ(10, fd_filesize(fd));
  p = mmap_private_fd(fd, &n);
  ASSERT_EQ(1, p != NULL);
  ASSERT_EQ(10, n);
  ASSERT_EQ('5', p[5]);
  p[5] = 'X'; /* private: the file is not changed */
  munmap(p, n);

  p = mmap_shared_fd(fd, &n);
  ASSERT_EQ(1, p != NULL);
  ASSERT_EQ('5', p[5]);
  p[0] = 'Z'; /* shared: the file is changed */
  munmap(p, n);
  close(fd);

  fd = open_read(fn);
  {
    char b[10];
    ASSERT_EQ(10, read(fd, b, 10));
    ASSERT_EQ(0, byte_diff(b, 10, "Z123456789"));
  }
  close(fd);

  p = (char*)mmap_readat(fn, &n, -100);
  ASSERT_EQ(1, p != NULL);
  ASSERT_EQ(10, n);
  ASSERT_EQ('Z', p[0]);
  munmap(p, n);
  unlink(fn);
}

#define RUN_GAPS_TESTS() \
  RUN(test_textcode_sa_array); \
  RUN(test_textcode_ldapescape2_scan_html); \
  RUN(test_range_inbuf); \
  RUN(test_wait_reap_nohang); \
  RUN(test_charbuf_skip_skipc); \
  RUN(test_hashmap_entry_delete2_rehash); \
  RUN(test_mmap_fd_variants)

TESTS(gaps) { RUN_GAPS_TESTS(); }
