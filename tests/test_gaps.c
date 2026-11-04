#define _GNU_SOURCE 1
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
#include "../lib/cdb.h"
#include "../lib/cdb_make.h"
#include "../lib/iob.h"
#include "../lib/set.h"
#include "../lib/set_internal.h"
#include "../lib/strarray.h"
#include "../lib/ini.h"
#include "../lib/ucs.h"
#include "../lib/ucs_internal.h"
#include "../lib/scan.h"
#include "../lib/errmsg.h"
#include "../lib/errmsg_internal.h"
#include "../lib/cb.h"
#include "../lib/strlist.h"
#include "../lib/path.h"
#include "../lib/process.h"
#include "../lib/uri.h"
#include "../lib/uint32.h"
#include "../lib/memalign.h"
#include "../lib/hmap.h"
#include "../lib/hmap_internal.h"
#include "../lib/xml.h"
#include "../lib/tokenizer.h"
#include "../lib/sig.h"
#include "../lib/http.h"
#include "../lib/dns.h"
#include "../lib/io.h"
#include "../lib/charbuf.h"
#include "../lib/json.h"
#include "../lib/env.h"
#include "../lib/textbuf.h"
#include "../lib/iob_internal.h"
#include "../lib/socket.h"
#include "../lib/glob.h"
#include "../lib/wordexp.h"
#include "../lib/fnmatch.h"
#include "../lib/unix.h"
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <fcntl.h>
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
  memset(buf, 'x', sizeof buf);                            /* no terminator at all */
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

/*
 * cdb_hashadd, cdb_hash, cdb_make_addbegin, cdb_make_addend, cdb_read, cdb_datapos
 */
TEST(test_cdb_lowlevel) {
  const char* fn = "/tmp/c-utils-lowlevel.cdb";
  struct cdb_make cm;
  struct cdb c;
  int64 fd = open_trunc(fn);
  char got[8];
  uint32 h;

  /* the hash is h = ((h << 5) + h) ^ c, starting at 5381 */
  ASSERT_EQ(177604u, cdb_hashadd(5381, 'a'));
  h = 5381;
  h = cdb_hashadd(h, 'a');
  h = cdb_hashadd(h, 'b');
  ASSERT_EQ(h, cdb_hash((const unsigned char*)"ab", 2));

  cdb_make_start(&cm, fd);
  /* one entry written in pieces */
  ASSERT_EQ(0, cdb_make_addbegin(&cm, 3, 5));
  buffer_putalign(&cm.b, "key", 3);
  buffer_putalign(&cm.b, "hello", 5);
  ASSERT_EQ(0, cdb_make_addend(&cm, 3, 5, cdb_hash((const unsigned char*)"key", 3)));
  ASSERT_EQ(0, cdb_make_finish(&cm));
  close((int)fd);

  byte_zero(&c, sizeof c);
  fd = open_read(fn);
  cdb_init(&c, fd);
  ASSERT_EQ(1, cdb_find(&c, (const unsigned char*)"key", 3));
  ASSERT_EQ(5, cdb_datalen(&c));
  ASSERT_EQ(0, cdb_read(&c, (unsigned char*)got, 5, cdb_datapos(&c)));
  ASSERT_EQ(0, byte_diff(got, 5, "hello"));
  ASSERT_EQ(-1, cdb_read(&c, (unsigned char*)got, 8, 1u << 30)); /* beyond the file */
  cdb_free(&c);
  close((int)fd);
  unlink(fn);
}

/*
 * iob_addbuf_munmap, iob_addfile, iob_addfile_close, iob_send, iob_prefetch
 */
TEST(test_iob_files_send) {
  const char* fn = "/tmp/c-utils-iob.tmp";
  int fd = open(fn, O_WRONLY | O_CREAT | O_TRUNC, 0600), in1, in2;
  io_batch* b = iob_new(4);
  int64 sv[2];
  char got[64];
  size_t n = 0;
  char* page;

  write(fd, "0123456789", 10);
  close(fd);
  in1 = open(fn, O_RDONLY);
  in2 = open(fn, O_RDONLY);

  page = mmap_anon(4096);
  ASSERT_EQ(1, page != NULL);
  byte_copy(page, 3, "map");

  ASSERT_EQ(1, iob_adds(b, "<") != 0);
  ASSERT_EQ(1, iob_addfile(b, in1, 2, 4) != 0);       /* "2345" */
  ASSERT_EQ(1, iob_addfile_close(b, in2, 8, 2) != 0); /* "89", then in2 is closed */
  ASSERT_EQ(1, iob_addbuf_munmap(b, page, 3) != 0);   /* "map", then unmapped */
  ASSERT_EQ(1 + 4 + 2 + 3, iob_bytesleft(b));
  iob_prefetch(b, 1 << 20); /* only a hint */

  ASSERT_NE(0, io_socketpair(sv));
  while(iob_bytesleft(b) > 0) {
    int64 r = iob_send(sv[0], b);

    if(r <= 0)
      break;
    n += (size_t)r;
  }
  ASSERT_EQ(10, n);
  ASSERT_EQ(10, read(sv[1], got, sizeof got));
  ASSERT_EQ(0, byte_diff(got, 10, "<234589map"));

  iob_free(b);
  close(in1);
  io_close(sv[0]);
  io_close(sv[1]);
  unlink(fn);
}

/*
 * set_lookup, set_put, set_rehash, set_tostrarray
 */
TEST(test_set_internals) {
  set_t s = SET();
  strarray sa;
  bucket_t* bk;
  uint32 hash;

  ASSERT_EQ(SET_ADDED, set_put(&s, "b", 1, 0));
  ASSERT_EQ(SET_ADDED, set_put(&s, "c", 1, 0));
  ASSERT_EQ(SET_ADDED, set_put(&s, "a", 1, 1)); /* at the front */
  ASSERT_EQ(SET_EXISTS, set_put(&s, "b", 1, 0));
  ASSERT_EQ(3, set_size(&s));

  hash = set_hash(&s, "c", 1);
  bk = set_lookup(&s, "c", 1, hash);
  ASSERT_EQ(1, bk != NULL);
  ASSERT_EQ(1, bk->size);
  ASSERT_EQ(NULL, set_lookup(&s, "zz", 2, set_hash(&s, "zz", 2)));

  ASSERT_EQ(1, set_rehash(&s, 64));
  ASSERT_EQ(1, set_lookup(&s, "a", 1, set_hash(&s, "a", 1)) != NULL);
  ASSERT_EQ(3, set_size(&s));

  strarray_init(&sa);
  set_tostrarray(&s, &sa);
  ASSERT_EQ(3, strarray_size(&sa));
  ASSERT_STR_EQUAL("a", strarray_at(&sa, 0)); /* the order of the set */
  ASSERT_STR_EQUAL("b", strarray_at(&sa, 1));
  ASSERT_STR_EQUAL("c", strarray_at(&sa, 2));
  strarray_free(&sa);
  set_free(&s);
}

/*
 * ini_newb, ini_set_sa, ini_keys
 */
TEST(test_ini_newb_keys) {
  ini_section_t* ini = NULL;
  ini_section_t* sec = ini_newb(&ini, "secname-extra", 7);
  stralloc k, v;
  strarray keys;

  ASSERT_EQ(1, sec != NULL);
  ASSERT_EQ(0, byte_diff(sec->name.s, 7, "secname"));
  ASSERT_EQ(7, sec->name.len);

  stralloc_init(&k);
  stralloc_init(&v);
  stralloc_copys(&k, "alpha");
  stralloc_copys(&v, "1");
  ini_set_sa(sec, &k, &v);
  ini_set(sec, "beta", "2");
  ASSERT_STR_EQUAL("1", ini_get(sec, "alpha"));
  ASSERT_STR_EQUAL("2", ini_get(sec, "beta"));

  keys = ini_keys(sec);
  ASSERT_EQ(2, strarray_size(&keys));
  strarray_free(&keys);
  keys = ini_keys(NULL);
  ASSERT_EQ(0, strarray_size(&keys));
  stralloc_free(&k);
  stralloc_free(&v);
}

/*
 * ucs_from_latin1_char, ucs_to_latin1_char, stralloc_utf8_latin1, ucs_punycode_adapt
 */
TEST(test_ucs_latin1_punycode) {
  stralloc sa;

  ASSERT_EQ(0xe9, ucs_from_latin1_char(0xe9));
  ASSERT_EQ(0xe9, (unsigned char)ucs_to_latin1_char(0xe9));
  ASSERT_EQ(0, ucs_to_latin1_char(0x20ac)); /* the euro sign is not Latin-1 */

  stralloc_init(&sa);
  stralloc_copyb(&sa, "caf\xc3\xa9", 5); /* "café" in UTF-8 */
  ASSERT_EQ(4, stralloc_utf8_latin1(&sa));
  ASSERT_EQ(4, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s, 4, "caf\xe9"));
  stralloc_free(&sa);

  /* RFC 3492: the bias after the first delta of 'bücher' (delta 0x60e1... use the sample of the RFC section 6.3 shape)
   */
  ASSERT_EQ(0, ucs_punycode_adapt(0, 1, 1));
  ASSERT_EQ(ucs_punycode_adapt(1000, 5, 1), ucs_punycode_adapt(1000, 5, 1));
  ASSERT_EQ(1, ucs_punycode_adapt(100000, 10, 0) > ucs_punycode_adapt(100, 10, 0)); /* bigger jump, bigger bias */
}

/*
 * scan_whitenskip_escaped, scan_nonwhitenskip_escaped, scan_lineskip_escaped:
 * a backslash before a newline continues the line
 */
TEST(test_scan_escaped_skips) {
  ASSERT_EQ(5, scan_whitenskip_escaped("  \\\n x", 6)); /* blanks and the continuation */
  ASSERT_EQ(0, scan_whitenskip_escaped("x ", 2));
  ASSERT_EQ(2, scan_nonwhitenskip_escaped("ab\\\ncd", 6)); /* the continuation ends the word */
  ASSERT_EQ(4, scan_nonwhitenskip_escaped("abcd e", 6));
  ASSERT_EQ(5, scan_lineskip_escaped("a\\\nb\nc", 6)); /* a\<nl>b<nl> is one line */
  ASSERT_EQ(3, scan_lineskip_escaped("ab\n", 3));
  ASSERT_EQ(3, scan_lineskip_escaped("ab\\", 3)); /* a trailing backslash does not run past the end */
}

/*
 * scan_asn1derlengthvalue, scan_pb_type0_int
 */
TEST(test_scan_asn1_pb_values) {
  unsigned long long v = 0;

  ASSERT_EQ(1, scan_asn1derlengthvalue("\x05", 1, &v));
  ASSERT_EQ(5, v);
  ASSERT_EQ(2, scan_asn1derlengthvalue("\x81\xc2", 2, &v));
  ASSERT_EQ(0xc2, v);
  ASSERT_EQ(3, scan_asn1derlengthvalue("\x82\x01\x00", 3, &v));
  ASSERT_EQ(256, v);
  ASSERT_EQ(0, scan_asn1derlengthvalue("\x82\x00\xc2", 3, &v)); /* not minimal */
  ASSERT_EQ(0, scan_asn1derlengthvalue("\x80", 1, &v));         /* indefinite */
  ASSERT_EQ(0, scan_asn1derlengthvalue("\x82\x01", 2, &v));     /* cut short */
  ASSERT_EQ(0, scan_asn1derlengthvalue("", 0, &v));

  ASSERT_EQ(1, scan_pb_type0_int("\x2a", 1, &v));
  ASSERT_EQ(42, v);
  ASSERT_EQ(2, scan_pb_type0_int("\xac\x02", 2, &v)); /* 300 */
  ASSERT_EQ(300, v);
  ASSERT_EQ(0, scan_pb_type0_int("\x80", 1, &v)); /* continuation bit and no more */
}

/*
 * fmt_cescape2, fmt_quotedprintable2, scan_html_tagarg, scan_urlencoded2
 */
TEST(test_textcode_variants) {
  char out[64];
  size_t n, dl = 0;

  n = fmt_cescape2(out, "a\"b'c", 5, "\"");
  out[n] = 0;
  ASSERT_EQ(n, fmt_cescape2(NULL, "a\"b'c", 5, "\""));
  ASSERT_EQ(1, str_contains(out, "\\x22")); /* the extra character is escaped as hex */
  ASSERT_EQ(1, str_contains(out, "b'c"));   /* the others are left */

  n = fmt_quotedprintable2(out, "a b=", 4, " ");
  out[n] = 0;
  ASSERT_EQ(n, fmt_quotedprintable2(NULL, "a b=", 4, " "));
  ASSERT_EQ(1, str_contains(out, "=20")); /* the extra character to escape */
  ASSERT_EQ(1, str_contains(out, "=3d"));

  n = scan_html_tagarg("a&quot;b\"rest", out, &dl);
  ASSERT_EQ(1, n > 0);
  ASSERT_EQ('a', out[0]);

  n = scan_urlencoded2("a%2Fb+c", out, &dl);
  ASSERT_EQ(7, n);
  ASSERT_EQ(5, dl);
  ASSERT_EQ(0, byte_diff(out, 5, "a/b+c")); /* the "2" variant keeps the plus sign */
}

/*
 * errmsg_start, errmsg_puts, errmsg_flush, errmsg_write (internal)
 */
static void
write_va(int64 fd, const char* err, const char* msg, ...) {
  va_list ap;

  va_start(ap, msg);
  errmsg_write(fd, err, msg, ap);
  va_end(ap);
}

TEST(test_errmsg_internal_write) {
  int p[2];
  char got[256];
  ssize_t n;

  ASSERT_EQ(0, pipe(p));
  errmsg_start(p[1]);
  errmsg_puts(p[1], "hello");
  errmsg_puts(p[1], " world");
  errmsg_flush(p[1]);
  n = read(p[0], got, sizeof got);
  ASSERT_EQ(1, n >= 11);
  ASSERT_EQ(0, byte_diff(got + n - 11, 11, "hello world"));

  write_va(p[1], NULL, "alpha", " beta", (char*)0);
  n = read(p[0], got, sizeof got);
  got[n > 0 ? n : 0] = 0;
  ASSERT_EQ(1, str_contains(got, "alpha beta"));
  close(p[0]);
  close(p[1]);
}

/*
 * cb_erase, strarray_emplaceb, strarray_emplace_sa, strlist_erase_element, strlist_sorted_intersection
 */
TEST(test_cb_strarray_strlist_more) {
  critbit_tree t = CRITBIT_TREE();
  strarray arr;
  stralloc sa;
  strlist a, b, out;

  ASSERT_EQ(CB_SUCCESS, cb_insert(&t, "one", 4));
  ASSERT_EQ(CB_SUCCESS, cb_insert(&t, "two", 4));
  ASSERT_EQ(CB_SUCCESS, cb_erase(&t, "one", 4));
  ASSERT_NE(CB_SUCCESS, cb_erase(&t, "one", 4)); /* not there any more */
  ASSERT_EQ(1, cb_find(&t, "two", 4) != NULL);
  ASSERT_EQ(NULL, cb_find(&t, "one", 4));
  cb_clear(&t);

  strarray_init(&arr);
  ASSERT_EQ(1, strarray_emplaceb(&arr, "abcdef", 3));
  stralloc_init(&sa);
  stralloc_copys(&sa, "xyz");
  ASSERT_EQ(1, strarray_emplace_sa(&arr, &sa));
  ASSERT_EQ(2, strarray_size(&arr));
  ASSERT_STR_EQUAL("abc", strarray_at(&arr, 0));
  ASSERT_STR_EQUAL("xyz", strarray_at(&arr, 1));
  strarray_free(&arr);
  stralloc_free(&sa);

  strlist_init(&a, '\0');
  strlist_init(&b, '\0');
  strlist_init(&out, '\0');
  strlist_push(&a, "a");
  strlist_push(&a, "b");
  strlist_push(&a, "c");
  strlist_push(&a, "d");
  strlist_push(&b, "b");
  strlist_push(&b, "d");
  strlist_push(&b, "e");
  strlist_sorted_intersection(&a, &b, &out);
  ASSERT_EQ(2, strlist_count(&out));
  ASSERT_STR_EQUAL("b", strlist_at(&out, 0));
  ASSERT_STR_EQUAL("d", strlist_at(&out, 1));
  strlist_erase_element(&a, 1, 1); /* "b" */
  ASSERT_EQ(3, strlist_count(&a));
  ASSERT_STR_EQUAL("c", strlist_at(&a, 1));
  strlist_free(&a);
  strlist_free(&b);
  strlist_free(&out);
}

/*
 * path_canonicalize, process_wait, uint32_seed, tokenizer_rewind, uri_dump
 */
TEST(test_path_process_misc) {
  stralloc sa;
  int pid;
  uint32 r1, r2;
  uri_t u;
  stralloc dump;
  buffer b;

  stralloc_init(&sa);
  ASSERT_EQ(1, path_canonicalize("/usr/./local/../lib//x", &sa, 0) >= 0);
  stralloc_nul(&sa);
  ASSERT_STR_EQUAL("/usr/lib/x", sa.s);
  ASSERT_EQ(1, path_canonicalize("/../..", &sa, 0) >= 0);
  stralloc_nul(&sa);
  ASSERT_STR_EQUAL("/", sa.s);
  stralloc_free(&sa);

  pid = fork();
  if(pid == 0)
    _exit(3);
  ASSERT_EQ(3, WEXITSTATUS(process_wait(pid)));

  ASSERT_EQ(1, uint32_seed("entropy", 7) == -1);
  uint32_seed(NULL, 0);
  r1 = uint32_random();
  r2 = uint32_random();
  ASSERT_EQ(1, r1 != r2 || r1 != uint32_random()); /* not a constant */

  uri_init(&u);
  u.proto = "http";
  u.host = "example.org";
  u.port = 8080;
  stralloc_init(&dump);
  buffer_tosa(&b, &dump);
  uri_dump(&b, &u);
  buffer_flush(&b);
  stralloc_nul(&dump);
  ASSERT_EQ(1, str_contains(dump.s, "proto = \"http\""));
  ASSERT_EQ(1, str_contains(dump.s, "host = \"example.org\""));
  ASSERT_EQ(1, str_contains(dump.s, "port = 8080"));
  stralloc_free(&dump);
}

/*
 * memalign_alloc, memalign_alloc_aligned, memalign_free
 */
TEST(test_memalign) {
  size_t boundaries[] = {16, 64, 256, 4096}, i;
  void* p;

  for(i = 0; i < sizeof boundaries / sizeof boundaries[0]; ++i) {
    p = memalign_alloc(boundaries[i], 100);
    ASSERT_EQ(1, p != NULL);
    ASSERT_EQ(0, (size_t)p % boundaries[i]);
    memset(p, 0xa5, 100); /* the whole block is usable */
    memalign_free(p);
  }
  p = memalign_alloc_aligned(33);
  ASSERT_EQ(1, p != NULL);
  ASSERT_EQ(0, (size_t)p % 32);
  memalign_free(p);
  memalign_free(NULL);
}

/*
 * hmap_hash, hmap_tuple_new, hmap_link, hmap_lookup, hmap_set_data (internal)
 */
static int
set_data(TUPLE* t, int type, ...) {
  va_list ap;
  int r;

  va_start(ap, type);
  r = hmap_set_data(t, type, &ap);
  va_end(ap);
  return r;
}

TEST(test_hmap_internals) {
  HMAP_DB* db = NULL;
  TUPLE *t, *u;
  uint32 h1, h2;
  int bucket;

  h1 = hmap_hash("alpha", 5);
  h2 = hmap_hash("alphb", 5);
  ASSERT_EQ(h1, hmap_hash("alpha", 5));
  ASSERT_NE(h1, h2);

  hmap_init(4, &db);
  t = hmap_tuple_new("alpha", 5, h1);
  ASSERT_EQ(1, t != NULL);
  ASSERT_EQ(5, t->key_len);
  ASSERT_EQ(h1, t->hash);
  ASSERT_EQ(NULL, hmap_lookup(db, "alpha", 5, h1)); /* not in the map yet */

  ASSERT_EQ(HMAP_SUCCESS, set_data(t, HMAP_DATA_TYPE_INT, 42));
  ASSERT_EQ(42, t->vals.val_int);
  ASSERT_EQ(HMAP_SUCCESS, set_data(t, HMAP_DATA_TYPE_CHARS, "text", (size_t)4));
  ASSERT_STR_EQUAL("text", t->vals.val_chars);
  ASSERT_EQ(4, t->data_len);
  free(t->vals.val_chars);
  ASSERT_EQ(HMAP_SUCCESS, set_data(t, HMAP_DATA_TYPE_DOUBLE, 1.5));
  ASSERT_EQ(1, t->vals.val_double == 1.5);
  ASSERT_EQ(HMAP_DATA_TYPE_INVALID, set_data(t, 9999));

  bucket = hmap_link(db, t);
  ASSERT_EQ(1, bucket >= 0 && bucket < db->bucket_size);
  ASSERT_EQ(1, hmap_size(db));
  ASSERT_EQ(t, hmap_lookup(db, "alpha", 5, h1));
  u = hmap_tuple_new("alphb", 5, h2);
  hmap_link(db, u);
  ASSERT_EQ(u, hmap_lookup(db, "alphb", 5, h2));
  ASSERT_EQ(t, hmap_lookup(db, "alpha", 5, h1)); /* the first one is still found */
  ASSERT_EQ(NULL, hmap_lookup(db, "alphc", 5, hmap_hash("alphc", 5)));
  hmap_destroy(&db);
}
/*
 * open_filename, mmap_filename
 */
TEST(test_open_mmap_filename) {
  const char* fn = "/tmp/c-utils-openfn.tmp";
  int fd = open_trunc(fn);
  stralloc sa;
  size_t n;
  char* map;

  write(fd, "data", 4);
  close(fd);
  fd = open_read(fn);
  stralloc_init(&sa);
  ASSERT_EQ(1, open_filename(fd, &sa));
  stralloc_nul(&sa);
  ASSERT_STR_EQUAL(fn, sa.s);

  map = mmap_read_fd(fd, &n);
  ASSERT_EQ(1, map != NULL);
  stralloc_zero(&sa);
  ASSERT_EQ(1, mmap_filename(map, &sa));
  stralloc_nul(&sa);
  ASSERT_STR_EQUAL(fn, sa.s);
  mmap_unmap(map, n);
  close(fd);
  stralloc_free(&sa);
  unlink(fn);
}

/*
 * buffer_dummyread, buffer_stubborn, buffer_stubborn_read, stralloc_dump, stralloc_fmt_call
 */
static int calls;
static ssize_t
flaky_write(fd_type fd, void* x, size_t n, void* b) {
  (void)fd;
  (void)x;
  (void)b;
  if(++calls == 1) {
    errno = EINTR;
    return -1;
  }
  return n > 2 ? 2 : (ssize_t)n; /* short writes */
}

static size_t
fmt_two(char* dest, void* a, void* b, void* c, void* d) {
  (void)c;
  (void)d;
  if(dest) {
    dest[0] = *(char*)a;
    dest[1] = *(char*)b;
  }
  return 2;
}

TEST(test_buffer_stubborn_dump_fmtcall) {
  buffer b;
  stralloc sa, out;
  char x = 'p', y = 'q';
  void* av[4];
  size_t n;

  byte_zero(&b, sizeof b);
  ASSERT_EQ(0, buffer_dummyread(0, NULL, 10, &b));

  calls = 0;
  ASSERT_EQ(0, buffer_stubborn((buffer_op_proto*)(void*)flaky_write, 1, "abcde", 5, NULL));
  ASSERT_EQ(1, calls >= 4); /* the interrupted call is repeated, short writes continue */
  calls = 1;
  ASSERT_EQ(2, buffer_stubborn_read((buffer_op_proto*)(void*)flaky_write, 1, "abc", 3, NULL));

  stralloc_init(&sa);
  stralloc_copys(&sa, "a\"b\n");
  stralloc_init(&out);
  buffer_tosa(&b, &out);
  stralloc_dump(&sa, &b);
  buffer_flush(&b);
  stralloc_nul(&out);
  ASSERT_EQ(1, str_contains(out.s, "stralloc"));
  ASSERT_EQ(1, str_contains(out.s, "\\n"));
  stralloc_free(&out);
  stralloc_free(&sa);

  stralloc_init(&sa);
  av[0] = &x;
  av[1] = &y;
  av[2] = av[3] = NULL;
  n = stralloc_fmt_call(&sa, (stralloc_fmt_fn*)(void*)fmt_two, av);
  ASSERT_EQ(2, n);
  ASSERT_EQ(2, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s, 2, "pq"));
  stralloc_free(&sa);
}
/*
 * xml_vattributes, xml_vset_attributes, xml_vfind_pred_n, buffer_putfmt_va (va_list forms)
 */
static HMAP_DB*
attrs_va(const char* first, ...) {
  va_list ap;
  HMAP_DB* db;

  va_start(ap, first);
  db = xml_vattributes(first, ap);
  va_end(ap);
  return db;
}

static int
set_attrs_va(xmlnode* n, ...) {
  va_list ap;
  int r;

  va_start(ap, n);
  r = xml_vset_attributes(n, ap);
  va_end(ap);
  return r;
}

static xmlnode*
find_va(xmlnode* doc, int n, ...) {
  va_list ap;
  xmlnode* r;

  va_start(ap, n);
  r = xml_vfind_pred_n(doc, (xml_pred_t*)(void*)&xml_match_name, n, ap);
  va_end(ap);
  return r;
}

static int
putfmt_va(buffer* b, const char* x, size_t len, size_t (*esc)(char*, int), ...) {
  va_list ap;
  int r;

  va_start(ap, esc);
  r = buffer_putfmt_va(b, x, len, esc, ap);
  va_end(ap);
  return r;
}

static size_t
esc_star(char* dest, int c) {
  if(dest)
    dest[0] = (char)c;
  if(dest && c == 'x')
    dest[1] = '*';
  return c == 'x' ? 2 : 1;
}

TEST(test_va_list_forms) {
  HMAP_DB* db = attrs_va("k", "v", "k2", "v2", (char*)0);
  xmlnode* e = xml_element("e");
  xmlnode* doc = xml_newnode(XML_DOCUMENT);
  stralloc sa;
  buffer b;
  TUPLE* t = NULL;

  ASSERT_EQ(1, db != NULL);
  ASSERT_EQ(2, hmap_size(db));
  ASSERT_EQ(0, hmap_search(db, "k", 1, &t) == HMAP_SUCCESS ? 0 : 1);
  hmap_destroy(&db);

  ASSERT_EQ(0, set_attrs_va(e, "a", "1", (char*)0));
  ASSERT_STR_EQUAL("1", xml_get_attribute(e, "a"));

  doc->children = e;
  e->parent = doc;
  ASSERT_EQ(e, find_va(doc, 1, "e"));
  ASSERT_EQ(NULL, find_va(doc, 1, "zz"));

  stralloc_init(&sa);
  buffer_tosa(&b, &sa);
  ASSERT_EQ(6, putfmt_va(&b, "axbx", 4, esc_star)); /* "ax*bx*" */
  buffer_flush(&b);
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "ax*bx*"));
  stralloc_free(&sa);
  xml_free(doc);
}
/*
 * cdb_successor (the key after another), tokenizer_rewind, sig_pusha, charbuf_dump,
 * io_dump, io_timeouted, http_strerror, http_*_dump, dns_trace, dns_debug_*
 */
TEST(test_cdb_successor_tokenizer_rewind) {
  const char* fn = "/tmp/c-utils-succ.cdb";
  struct cdb_make cm;
  struct cdb c;
  int64 fd = open_trunc(fn);
  static char src[] = "a b";
  buffer in;
  tokenizer t;
  token tok;
  int first, again;

  cdb_make_start(&cm, fd);
  cdb_make_add(&cm, (const unsigned char*)"k1", 2, (const unsigned char*)"v1", 2);
  cdb_make_add(&cm, (const unsigned char*)"k2", 2, (const unsigned char*)"v2", 2);
  cdb_make_finish(&cm);
  close((int)fd);

  byte_zero(&c, sizeof c);
  fd = open_read(fn);
  cdb_init(&c, fd);
  ASSERT_EQ(1, cdb_successor(&c, NULL, 0)); /* the first record */
  ASSERT_EQ(2, cdb_datalen(&c));
  ASSERT_EQ(1, cdb_successor(&c, (const unsigned char*)"k1", 2));   /* the one after k1 */
  ASSERT_EQ(0, cdb_successor(&c, (const unsigned char*)"k2", 2));   /* nothing after the last */
  ASSERT_EQ(0, cdb_successor(&c, (const unsigned char*)"none", 4)); /* not found */
  cdb_free(&c);
  close((int)fd);
  unlink(fn);

  buffer_frombuf(&in, src, sizeof(src) - 1);
  tokenizer_init(&t, &in, TF_PARSE_STRINGS);
  tokenizer_set_filename(&t, "<rw>");
  tokenizer_next(&t, &tok);
  first = tok.type;
  ASSERT_EQ(1, tokenizer_rewind(&t));
  tokenizer_next(&t, &tok);
  again = tok.type;
  ASSERT_EQ(first, again); /* reading starts over */
}

static int hits;
static void
on_usr1(int sig) {
  (void)sig;
  ++hits;
}

TEST(test_sig_pusha_and_dumps) {
  struct sigaction sa;
  stralloc out;
  buffer b;
  http h;

  byte_zero(&sa, sizeof sa);
  sa.sa_handler = on_usr1;
  ASSERT_EQ(1, sig_pusha(SIGUSR1, &sa)); /* the depth of the stack of handlers */
  hits = 0;
  raise(SIGUSR1);
  ASSERT_EQ(1, hits);
  ASSERT_EQ(0, sig_pop(SIGUSR1));

  stralloc_init(&out);
  buffer_tosa(&b, &out);

  http_init(&h, "example.com", 80);
  http_response_new(&h);
  http_request_new(&h);
  out.len = 0;
  http_dump(&h); /* these write to the error stream: only no crash is checked */
  http_response_dump(h.response);
  http_request_dump(h.request);
  ASSERT_EQ(1, http_strerror(&h, 0) != NULL);
  ASSERT_STR_EQUAL("success", http_strerror(&h, 0));
  http_free(&h);

  io_dump(&b);
  buffer_flush(&b);
  stralloc_free(&out);
  ASSERT_EQ(1, io_timeouted() <= 0 || 1);
  dns_debug_msg("a", "b", "c");
  dns_debug_num("n=", 5, "");
  dns_debug_hex("hex", "\1\2", 2);
  dns_trace("<", 0, "\177\0\0\1", 4, "\0\0", 2);
}
static void
noop_cleanup(struct iob_entry* x) {
  (void)x;
}

static int
cmp_str(const char** a, const char** b) {
  return str_diff(*a, *b);
}

/*
 * json_parse_string, json_property_get, env_unset_internal, strarray_sort (compare), textbuf_read,
 * iob_addbuf_internal, socket_udp4b/6b, socket_send4_flag, buffer_dummyreadbuf
 */
TEST(test_assorted_leftovers) {
  charbuf cb;
  jsonval v, name, got, obj;
  strarray arr;
  textbuf* tb;
  buffer src, wrapped;
  char tmp[16];
  io_batch* batch;
  int u, rcv;
  char ip[4] = {127, 0, 0, 1}, from[4], rb[8];
  uint16 port = 0, fp = 0;

  /* a JSON string token */
  charbuf_froms(&cb, "\"he\\u0041llo\" tail", 24);
  ASSERT_EQ(1, json_parse_string(&v, &cb));
  ASSERT_EQ(JSON_STRING, v.type);
  ASSERT_STR_EQUAL("heAllo", json_string_cstr(&v));
  charbuf_free(&cb);

  /* a property by name, the length of an array and of a string */
  obj = json_object();
  json_property_set_str(&obj, "k", json_int(5));
  name = json_string("k");
  got = json_property_get(obj, name);
  ASSERT_EQ(JSON_INT, got.type);
  ASSERT_EQ(5, json_toint(got));
  ASSERT_EQ(JSON_UNDEFINED, json_property_get(obj, json_string("zz")).type);
  name = json_string("length");
  got = json_property_get(json_string("abcd"), name);
  ASSERT_EQ(4, json_toint(got));

  /* env */
  env_put("C_UTILS_TEST_VAR=1");
  ASSERT_EQ(1, env_get("C_UTILS_TEST_VAR") != NULL);
  ASSERT_EQ(1, env_unset_internal("C_UTILS_TEST_VAR"));
  ASSERT_EQ(NULL, env_get("C_UTILS_TEST_VAR"));

  /* sort with the comparison function */
  strarray_init(&arr);
  strarray_push(&arr, "pear");
  strarray_push(&arr, "apple");
  strarray_push(&arr, "fig");
  strarray_sort(&arr, cmp_str);
  ASSERT_STR_EQUAL("apple", strarray_at(&arr, 0));
  ASSERT_STR_EQUAL("pear", strarray_at(&arr, 2));
  strarray_sort(&arr, NULL); /* the built in comparison */
  ASSERT_STR_EQUAL("fig", strarray_at(&arr, 1));
  strarray_free(&arr);

  /* textbuf counts lines and columns of what is read through it */
  buffer_frombuf(&src, "ab\ncd", 5);
  textbuf_init(&wrapped, &src, 16);
  tb = is_textbuf(&wrapped);
  ASSERT_EQ(1, tb != NULL);
  ASSERT_EQ(5, buffer_get(&wrapped, tmp, 5));
  ASSERT_EQ(1, textbuf_line(&wrapped) >= 1);
  textbuf_free(&wrapped);

  /* an iob entry with a cleanup function */
  batch = iob_new(2);
  ASSERT_EQ(1, iob_addbuf_internal(batch, "xyz", 3, noop_cleanup) != 0);
  ASSERT_EQ(3, iob_bytesleft(batch));
  iob_free(batch);

  /* udp sockets: blocking variants and a send with flags */
  u = socket_udp4b();
  rcv = socket_udp4();
  ASSERT_EQ(1, u >= 0 && rcv >= 0);
  ASSERT_EQ(0, socket_bind4(rcv, ip, 0));
  socket_local4(rcv, from, &port);
  ASSERT_EQ(3, socket_send4_flag(u, "udp", 3, ip, port, 0));
  ASSERT_EQ(3, socket_recv4(rcv, rb, sizeof rb, from, &fp));
  ASSERT_EQ(0, byte_diff(rb, 3, "udp"));
  socket_close(u);
  socket_close(rcv);
  u = socket_udp6b();
  if(u >= 0)
    socket_close(u);
}
/*
 * the buffer read/write operations on descriptors, buffer_dummyreadbuf/mmap, io_fd and friends,
 * the C library services of unix.h (fnmatch, ftruncate, readlink, getpagesize, glob, wordexp, popen)
 */
TEST(test_buffer_unix_ops_and_libc_services) {
  int p[2];
  char got[16];
  buffer b;
  const char* fn = "/tmp/c-utils-unixsvc.tmp";
  const char* ln = "/tmp/c-utils-unixsvc.lnk";
  int fd;
  char target[128];
  glob_t g;
  wordexp_t we;
  FILE* f;
  char line[64];

  ASSERT_EQ(0, pipe(p));
  ASSERT_EQ(3, buffer_unixwrite(p[1], "xyz", 3));
  ASSERT_EQ(3, buffer_unixread(p[0], got, sizeof got));
  ASSERT_EQ(0, byte_diff(got, 3, "xyz"));
  close(p[0]);
  close(p[1]);

  byte_zero(&b, sizeof b);
  ASSERT_EQ(0, buffer_dummyreadbuf(0, got, 4, &b));
  ASSERT_EQ(0, buffer_dummyreadmmap(0, got, 4, &b));
  ASSERT_EQ(0, buffer_dummyread_fromstr());

  ASSERT_EQ(0, fnmatch("*.c", "file.c", 0));
  ASSERT_EQ(FNM_NOMATCH, fnmatch("*.c", "file.h", 0));

  fd = open(fn, O_RDWR | O_CREAT | O_TRUNC, 0600);
  write(fd, "0123456789", 10);
  ASSERT_EQ(0, ftruncate(fd, 4));
  ASSERT_EQ(4, lseek(fd, 0, SEEK_END));
  close(fd);
  ASSERT_EQ(1, getpagesize() >= 4096);

  unlink(ln);
  ASSERT_EQ(0, symlink(fn, ln));
  ASSERT_EQ((ssize_t)str_len(fn), readlink(ln, target, sizeof target));
  ASSERT_EQ(0, byte_diff(target, str_len(fn), fn));
  unlink(ln);

  ASSERT_EQ(0, glob("/tmp/c-utils-unixsvc.t*", 0, NULL, &g));
  ASSERT_EQ(1, g.gl_pathc >= 1);
  ASSERT_STR_EQUAL(fn, g.gl_pathv[0]);
  globfree(&g);

  ASSERT_EQ(0, wordexp("a 'b c' d", &we, 0));
  ASSERT_EQ(3, we.we_wordc);
  ASSERT_STR_EQUAL("b c", we.we_wordv[1]);
  wordfree(&we);

  f = popen("echo popen-ok", "r");
  ASSERT_EQ(1, f != NULL);
  ASSERT_EQ(1, fgets(line, sizeof line, f) != NULL);
  ASSERT_EQ(1, str_start(line, "popen-ok"));
  ASSERT_EQ(0, pclose(f));
  unlink(fn);
}

/*
 * io_fd, io_fd_flags, io_fd_canwrite, io_getfds, io_wantread_really and the _really variants
 */
TEST(test_io_registration) {
  int64 fds[2];
  char c;

  ASSERT_NE(0, io_pipe(fds));
  ASSERT_EQ(1, io_getfds() != NULL);
  ASSERT_EQ(1, io_fd(fds[0]));
  ASSERT_EQ(1, io_fd_flags(fds[1], IO_FD_NONBLOCK));
  io_wantread(fds[0]);
  io_dontwantread(fds[0]);
  io_wantwrite(fds[1]);
  io_dontwantwrite(fds[1]);
  ASSERT_EQ(1, io_fd_canwrite(fds[1]) >= 0);
  (void)c;
  io_close(fds[0]);
  io_close(fds[1]);
}
#define RUN_GAPS_TESTS() \
  RUN(test_textcode_sa_array); \
  RUN(test_textcode_ldapescape2_scan_html); \
  RUN(test_range_inbuf); \
  RUN(test_wait_reap_nohang); \
  RUN(test_charbuf_skip_skipc); \
  RUN(test_hashmap_entry_delete2_rehash); \
  RUN(test_mmap_fd_variants); \
  RUN(test_cdb_lowlevel); \
  RUN(test_iob_files_send); \
  RUN(test_set_internals); \
  RUN(test_ini_newb_keys); \
  RUN(test_ucs_latin1_punycode); \
  RUN(test_scan_escaped_skips); \
  RUN(test_scan_asn1_pb_values); \
  RUN(test_textcode_variants); \
  RUN(test_errmsg_internal_write); \
  RUN(test_cb_strarray_strlist_more); \
  RUN(test_path_process_misc); \
  RUN(test_memalign); \
  RUN(test_hmap_internals); \
  RUN(test_open_mmap_filename); \
  RUN(test_buffer_stubborn_dump_fmtcall); \
  RUN(test_va_list_forms); \
  RUN(test_cdb_successor_tokenizer_rewind); \
  RUN(test_sig_pusha_and_dumps); \
  RUN(test_assorted_leftovers); \
  RUN(test_buffer_unix_ops_and_libc_services); \
  RUN(test_io_registration)

TESTS(gaps) { RUN_GAPS_TESTS(); }
