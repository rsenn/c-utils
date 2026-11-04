#include "../lib/alloc.h"
#include "../lib/byte.h"
#include "../lib/stralloc.h"
#include "../lib/array.h"
#include "../lib/uint64.h"
#include "unit_test.h"
#include "../lib/buffer.h"
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <fcntl.h>
#include <unistd.h>
#include "../lib/open.h"
#include "../lib/tai.h"

/* a buffer that collects what is written to it in a stralloc */
static int
tp_end_nl(const char* x, size_t n) {
  return n > 0 && x[n - 1] == '\n';
}
static int
tp_end_arg(const char* x, size_t n, void* arg) {
  return n > 0 && x[n - 1] == *(char*)arg;
}
static int
sp_end_semicolon(stralloc* sa) {
  return sa->len > 0 && sa->s[sa->len - 1] == ';';
}
static int
sp_end_arg(stralloc* sa, void* arg) {
  return sa->len > 0 && sa->s[sa->len - 1] == *(char*)arg;
}
static size_t
esc_upper_hex(char* dest, int c) {
  if(c >= 'a' && c <= 'z') {
    if(dest) {
      dest[0] = '<';
      dest[1] = (char)(c - 32);
      dest[2] = '>';
    }
    return 3;
  }
  if(dest)
    dest[0] = (char)c;
  return 1;
}
static size_t
esc_with_arg(char* dest, int c, void* a, void* b, void* c3, void* d) {
  (void)c3;
  (void)d;
  dest[0] = (char)c;
  dest[1] = *(char*)a;
  dest[2] = *(char*)b;
  return 3;
}

#define TESTFILE "/tmp/test_buffer_unit.dat"

static void
write_testfile(const char* s) {
  int fd = open_trunc(TESTFILE);

  if(fd != -1) {
    if(write(fd, s, str_len(s)) < 0)
      fd = fd;
    close(fd);
  }
}

static size_t
read_testfile(char* buf, size_t max) {
  int fd = open_read(TESTFILE);
  ssize_t n = fd == -1 ? 0 : read(fd, buf, max);

  if(fd != -1)
    close(fd);
  return n < 0 ? 0 : (size_t)n;
}

/* Discards whatever buffer_flush() writes; used so OUTBUF's op is
   never a NULL pointer (buffer_stubborn() does not guard against
   that), even though these tests are sized so a flush is never
   actually needed except where one is exercised explicitly. */
static ssize_t
discard_write(fd_type fd, void* buf, size_t len, buffer* b) {
  (void)fd;
  (void)buf;
  (void)b;
  return (ssize_t)len;
}

/* Fixed-size output sink big enough that none of these tests ever need
   to actually flush. */
#define OUTBUF(name) \
  char name##_x[4096]; \
  buffer name = BUFFER_INIT(&discard_write, -1, name##_x, sizeof(name##_x))

/*
 * void buffer_init(buffer*, buffer_op_proto*, fd_type fd, char* y, size_t ylen);
 */
TEST(test_buffer_init) {
  char x[16];
  buffer b;

  buffer_init(&b, 0, -1, x, sizeof(x));
  ASSERT_EQ((size_t)x, (size_t)b.x);
  ASSERT_EQ(sizeof(x), b.a);
  ASSERT_EQ(0, b.p);
  ASSERT_EQ(0, b.n);
}

/*
 * void buffer_init_free(buffer*, buffer_op_proto*, fd_type fd, char* y, size_t ylen);
 */
TEST(test_buffer_init_free) {
  char* x = alloc(16);
  buffer b;

  buffer_init_free(&b, 0, -1, x, 16);
  ASSERT_EQ((size_t)&buffer_free, (size_t)b.deinit);
  buffer_free(&b);
}

/*
 * void buffer_free(buffer*);
 */
TEST(test_buffer_free) {
  char* x = alloc(4);
  buffer b;

  buffer_init_free(&b, 0, -1, x, 4);
  buffer_free(&b);
}

/*
 * void buffer_munmap(buffer*);
 */
TEST(test_buffer_munmap) {
  buffer b;

  write_testfile("hello file");
  ASSERT_EQ(0, buffer_mmapread(&b, TESTFILE));
  ASSERT_EQ(10, b.n);
  buffer_munmap(&b);
  ASSERT_EQ(1, b.x == NULL);
  ASSERT_EQ(0, b.n); /* reads as empty, not as a dangling mapping */
}

/*
 * int buffer_mmapread(buffer*, const char* filename);
 */
TEST(test_buffer_mmapread) {
  buffer b;
  char out[16];

  write_testfile("hello file");
  ASSERT_EQ(0, buffer_mmapread(&b, TESTFILE));
  ASSERT_EQ(10, b.n);
  ASSERT_EQ(5, buffer_get(&b, out, 5));
  ASSERT_EQ(0, byte_diff(out, 5, "hello"));
  buffer_close(&b);
  ASSERT_EQ(-1, buffer_mmapread(&b, "/nonexistent/dir/file"));
  ASSERT_EQ(0, b.n); /* a failed map leaves an empty buffer */
}

/*
 * int buffer_mmapread_fd(buffer*, fd_type fd);
 */
TEST(test_buffer_mmapread_fd) {
  buffer b;
  int fd;

  write_testfile("hello file");
  fd = open_read(TESTFILE);
  ASSERT_EQ(0, buffer_mmapread_fd(&b, fd));
  ASSERT_EQ(10, b.n);
  ASSERT_EQ(0, byte_diff(b.x, 10, "hello file"));
  buffer_close(&b); /* the buffer owns fd after a successful call */
}

/*
 * int buffer_mmapprivate(buffer*, const char* filename);
 */
TEST(test_buffer_mmapprivate) {
  buffer b;
  char t[16];

  write_testfile("hello");
  ASSERT_EQ(0, buffer_mmapprivate(&b, TESTFILE));
  b.x[0] = 'J'; /* copy on write: the file does not change */
  ASSERT_EQ('J', b.x[0]);
  buffer_close(&b);
  ASSERT_EQ(5, read_testfile(t, 16));
  ASSERT_EQ('h', t[0]);
}

/*
 * int buffer_mmapprivate_fd(buffer*, fd_type fd);
 */
TEST(test_buffer_mmapprivate_fd) {
  buffer b;
  int fd;

  write_testfile("hello");
  fd = open_read(TESTFILE);
  ASSERT_EQ(0, buffer_mmapprivate_fd(&b, fd));
  ASSERT_EQ(5, b.n);
  buffer_close(&b); /* the buffer owns fd after a successful call */
}

/*
 * int buffer_mmapshared(buffer*, const char* filename);
 *
 * Not tested: declared in buffer.h but has no implementation anywhere
 * in the tree, unlike its buffer_mmapread/buffer_mmapprivate siblings
 * which each have one; only buffer_mmapshared_fd exists. Would fail to
 * link. See BUGS.
 */
TEST(test_buffer_mmapshared) {
  buffer b;
  char t[16];

  write_testfile("hello");
  ASSERT_EQ(0, buffer_mmapshared(&b, TESTFILE));
  ASSERT_EQ(5, b.n);
  b.x[0] = 'J'; /* shared: the change reaches the file */
  buffer_close(&b);
  ASSERT_EQ(5, read_testfile(t, 16));
  ASSERT_EQ('J', t[0]);
  ASSERT_EQ(-1, buffer_mmapshared(&b, "/nonexistent/dir/file"));
}

/*
 * int buffer_mmapshared_fd(buffer*, fd_type fd);
 */
TEST(test_buffer_mmapshared_fd) {
  buffer b;
  int fd;
  char t[16];

  write_testfile("hello");
  fd = open_rw(TESTFILE);
  ASSERT_EQ(0, buffer_mmapshared_fd(&b, fd));
  b.x[1] = 'E';
  buffer_close(&b); /* the buffer owns fd after a successful call */
  read_testfile(t, 16);
  ASSERT_EQ('E', t[1]);
}

/*
 * void buffer_close(buffer* b);
 */
TEST(test_buffer_close) {
  buffer b;
  int fd;

  write_testfile("x");
  fd = open_read(TESTFILE);
  ASSERT_EQ(0, buffer_mmapread_fd(&b, fd));
  ASSERT_NE(-1, fcntl(fd, F_GETFD));
  buffer_close(&b); /* closes the descriptor the buffer owns */
  ASSERT_EQ(-1, fcntl(fd, F_GETFD));
}

/*
 * int buffer_read_fd(buffer*, fd_type fd);
 */
TEST(test_buffer_read_fd) {
  buffer b;
  char out[16];
  int fd;

  write_testfile("hello file");
  fd = open_read(TESTFILE);
  ASSERT_EQ(0, buffer_read_fd(&b, fd));
  ASSERT_EQ(10, buffer_get(&b, out, 16));
  ASSERT_EQ(0, byte_diff(out, 10, "hello file"));
  ASSERT_EQ(0, buffer_get(&b, out, 16));
  buffer_free(&b);
  close(fd);
}

/*
 * int buffer_flush(buffer* b);
 */
TEST(test_buffer_flush) {
  OUTBUF(b);

  buffer_puts(&b, "hi");
  ASSERT_EQ(0, buffer_flush(&b));
}

/*
 * int buffer_put(buffer*, const char* x, size_t len);
 */
TEST(test_buffer_put) {
  OUTBUF(b);

  buffer_put(&b, "hello", 5);
  ASSERT_EQ(5, b.p);
  ASSERT_EQ(0, byte_diff(b.x, 5, "hello"));
}

/*
 * int buffer_putalign(buffer*, const char* x, size_t len);
 */
TEST(test_buffer_putalign) {
  OUTBUF(b);

  buffer_putalign(&b, "hello", 5);
  ASSERT_EQ(5, b.p);
  ASSERT_EQ(0, byte_diff(b.x, 5, "hello"));
}

/*
 * ssize_t buffer_putflush(buffer*, const char* x, size_t len);
 */
TEST(test_buffer_putflush) {
  buffer b;
  char mem[32], t[16];
  int fd = open_trunc(TESTFILE);

  buffer_init(&b, (buffer_op_ptr)write, fd, mem, sizeof mem);
  ASSERT_EQ(0, buffer_putflush(&b, "pf\n", 3));
  ASSERT_EQ(0, b.p);
  close(fd);
  ASSERT_EQ(3, read_testfile(t, 16));
}

/*
 * int buffer_puts(buffer*, const char* x);
 */
TEST(test_buffer_puts) {
  OUTBUF(b);

  buffer_puts(&b, "hello");
  ASSERT_EQ(5, b.p);
  ASSERT_EQ(0, byte_diff(b.x, 5, "hello"));
}

/*
 * int buffer_putsalign(buffer*, const char* x);
 */
TEST(test_buffer_putsalign) {
  OUTBUF(b);

  buffer_putsalign(&b, "hello");
  ASSERT_EQ(5, b.p);
}

/*
 * ssize_t buffer_putsflush(buffer*, const char* x);
 */
TEST(test_buffer_putsflush) {
  buffer b;
  char mem[32], t[16];
  int fd = open_trunc(TESTFILE);

  buffer_init(&b, (buffer_op_ptr)write, fd, mem, sizeof mem);
  ASSERT_EQ(0, buffer_putsflush(&b, "psf"));
  ASSERT_EQ(0, b.p);
  close(fd);
  ASSERT_EQ(3, read_testfile(t, 16));
}

/*
 * int buffer_putm_internal(buffer* b, ...);
 */
TEST(test_buffer_putm_internal) {
  OUTBUF(b);

  buffer_putm_internal(&b, "foo", "bar", "baz", NULL);
  ASSERT_EQ(9, b.p);
  ASSERT_EQ(0, byte_diff(b.x, 9, "foobarbaz"));
}

/*
 * int buffer_putm_internal_flush(buffer* b, ...);
 */
TEST(test_buffer_putm_internal_flush) {
  buffer b;
  char mem[32], t[16];
  int fd = open_trunc(TESTFILE);

  buffer_init(&b, (buffer_op_ptr)write, fd, mem, sizeof mem);
  ASSERT_EQ(0, buffer_putm_internal_flush(&b, "a", "b", "c", (char*)0));
  close(fd);
  ASSERT_EQ(3, read_testfile(t, 16));
  ASSERT_EQ(0, byte_diff(t, 3, "abc"));
}

/*
 * int buffer_putspace(buffer*);
 */
TEST(test_buffer_putspace) {
  OUTBUF(b);

  buffer_putspace(&b);
  ASSERT_EQ(1, b.p);
  ASSERT_EQ(' ', b.x[0]);
}

/*
 * int buffer_putnlflush(buffer*);
 */
TEST(test_buffer_putnlflush) {
  buffer b;
  char mem[32], t[16];
  int fd = open_trunc(TESTFILE);

  buffer_init(&b, (buffer_op_ptr)write, fd, mem, sizeof mem);
  buffer_puts(&b, "line");
  ASSERT_EQ(0, buffer_putnlflush(&b));
  close(fd);
  ASSERT_EQ(5, read_testfile(t, 16));
  ASSERT_EQ('\n', t[4]);
}

/*
 * ssize_t buffer_get(buffer*, char*, size_t);
 */
TEST(test_buffer_get) {
  buffer in;
  char out[8] = {0};

  buffer_frombuf(&in, "hello world", 11);
  ASSERT_EQ(5, buffer_get(&in, out, 5));
  ASSERT_EQ(0, byte_diff(out, 5, "hello"));
  ASSERT_EQ(5, in.p);
}

/*
 * ssize_t buffer_getc(buffer*, char*);
 */
TEST(test_buffer_getc) {
  buffer in;
  char c = 0;

  buffer_frombuf(&in, "ab", 2);
  ASSERT_EQ(1, buffer_getc(&in, &c));
  ASSERT_EQ('a', c);
  ASSERT_EQ(1, buffer_getc(&in, &c));
  ASSERT_EQ('b', c);
  ASSERT_EQ(0, buffer_getc(&in, &c));
}

/*
 * ssize_t buffer_getn(buffer*, char*, size_t);
 */
TEST(test_buffer_getn) {
  buffer in;
  char out[4] = {0};

  buffer_frombuf(&in, "abcd", 4);
  ASSERT_EQ(4, buffer_getn(&in, out, 4));
  ASSERT_EQ(0, byte_diff(out, 4, "abcd"));
}

/*
 * ssize_t buffer_get_token(buffer*, char*, size_t, const char*, size_t);
 */
TEST(test_buffer_get_token) {
  buffer in;
  char out[32] = {0};

  buffer_frombuf(&in, "foo,bar,baz", 11);
  ASSERT_EQ(4, buffer_get_token(&in, out, sizeof(out), ",", 1));
  ASSERT_EQ(0, byte_diff(out, 4, "foo,"));
}

/*
 * ssize_t buffer_getline(buffer*, char*, size_t);
 */
TEST(test_buffer_getline) {
  buffer in;
  char out[32] = {0};

  buffer_frombuf(&in, "line1\nline2", 11);
  ASSERT_EQ(6, buffer_getline(&in, out, sizeof(out)));
  ASSERT_EQ(0, byte_diff(out, 6, "line1\n"));
}

/*
 * int buffer_skip_until(buffer*, const char*, size_t);
 */
TEST(test_buffer_skip_until) {
  buffer in;
  char out[8] = {0};

  buffer_frombuf(&in, "abc,def", 7);
  ASSERT_EQ(4, buffer_skip_until(&in, ",", 1));
  ASSERT_EQ(3, buffer_get(&in, out, 3));
  ASSERT_EQ(0, byte_diff(out, 3, "def"));
}

/*
 * typedef int (*string_predicate)(const char*, size_t, void*);
 * ssize_t buffer_get_token_pred(buffer*, char*, size_t, string_predicate, void*);
 */
TEST(test_buffer_get_token_pred) {
  buffer b;
  char out[32];
  char term = ';';
  char text[] = "ab\ncd;ef";

  buffer_fromstr(&b, text, sizeof(text) - 1);
  /* the terminator is stored after the token but not counted */
  ASSERT_EQ(2, buffer_get_token_pred(&b, out, sizeof out, tp_end_nl));
  ASSERT_EQ(0, byte_diff(out, 3, "ab\n"));
  ASSERT_EQ(2, buffer_get_token_pred_arg(&b, out, sizeof out, tp_end_arg, &term));
  ASSERT_EQ(0, byte_diff(out, 3, "cd;"));
  /* the rest has no terminator: it ends with the input */
  ASSERT_EQ(2, buffer_get_token_pred(&b, out, sizeof out, tp_end_nl));
  ASSERT_EQ(0, byte_diff(out, 2, "ef"));
  ASSERT_EQ(0, buffer_get_token_pred(&b, out, sizeof out, tp_end_nl));
}

/*
 * char* buffer_peek(buffer*);
 */
TEST(test_buffer_peek) {
  buffer in;

  buffer_frombuf(&in, "abc", 3);
  ASSERT_EQ('a', *buffer_peek(&in));
  ASSERT_EQ(0, in.p); /* peek must not consume */
}

/*
 * int buffer_peekc(buffer*, char*);
 */
TEST(test_buffer_peekc) {
  buffer in;
  char c = 0;

  buffer_frombuf(&in, "abc", 3);
  ASSERT_EQ(1, buffer_peekc(&in, &c));
  ASSERT_EQ('a', c);
  ASSERT_EQ(0, in.p);
}

/*
 * void buffer_seek(buffer*, size_t);
 */
TEST(test_buffer_seek) {
  buffer in;

  buffer_frombuf(&in, "abcdef", 6);
  buffer_seek(&in, 3);
  ASSERT_EQ(3, in.p);
}

/*
 * void buffer_rewind(buffer*);
 */
TEST(test_buffer_rewind) {
  buffer in;
  char c;

  buffer_frombuf(&in, "abcdef", 6);
  buffer_getc(&in, &c);
  buffer_getc(&in, &c);
  buffer_rewind(&in);
  ASSERT_EQ(0, in.p);
}

/*
 * int buffer_skipc(buffer*);
 */
TEST(test_buffer_skipc) {
  buffer in;
  char c;

  buffer_frombuf(&in, "abc", 3);
  ASSERT_EQ(1, buffer_skipc(&in));
  buffer_getc(&in, &c);
  ASSERT_EQ('b', c);
}

/*
 * int buffer_skipn(buffer*, size_t);
 */
TEST(test_buffer_skipn) {
  buffer in;
  char c;

  buffer_frombuf(&in, "abcdef", 6);
  ASSERT_EQ(3, buffer_skipn(&in, 3));
  buffer_getc(&in, &c);
  ASSERT_EQ('d', c);
}

/*
 * buffer_EOF / buffer_PEEK / buffer_LEN / buffer_SKIP / buffer_SEEK / buffer_EMPTY
 */
TEST(test_buffer_macros) {
  buffer in;

  buffer_frombuf(&in, "abcdef", 6);
  ASSERT_EQ(6, buffer_LEN(&in));
  ASSERT_EQ(0, buffer_EMPTY(&in));
  ASSERT_EQ('a', *buffer_PEEK(&in));

  buffer_SKIP(&in, 6);
  ASSERT_EQ(1, buffer_EMPTY(&in));
  ASSERT_EQ(1, buffer_EOF(&in));
}

/*
 * buffer_GETC
 */
TEST(test_buffer_GETC) {
  buffer in;
  char c;

  buffer_frombuf(&in, "xy", 2);
  ASSERT_EQ(1, buffer_GETC(&in, &c));
  ASSERT_EQ('x', c);
}

/*
 * buffer_BEGIN / buffer_END / buffer_HEADROOM / buffer_SPACE
 */
TEST(test_buffer_region_macros) {
  OUTBUF(b);

  ASSERT_EQ((size_t)b.x, (size_t)buffer_BEGIN(&b));
  ASSERT_EQ((size_t)(b.x + b.n), (size_t)buffer_END(&b));
  ASSERT_EQ(b.a - b.n, buffer_HEADROOM(&b));
  ASSERT_EQ(b.a - b.p, buffer_SPACE(&b));
}

/*
 * buffer_MOVE / buffer_SCAN
 */
TEST(test_buffer_move_scan) {
  char src[6];
  buffer in;

  byte_copy(src, 6, "abcdef");
  buffer_frombuf(&in, src, 6);
  buffer_SKIP(&in, 2);
  buffer_MOVE(&in);
  ASSERT_EQ(0, in.p);
  ASSERT_EQ(4, in.n);
  ASSERT_EQ(0, byte_diff(in.x, 4, "cdef"));
}

/*
 * int buffer_putulong(buffer*, unsigned long);
 */
TEST(test_buffer_putulong) {
  OUTBUF(b);

  buffer_putulong(&b, 12345);
  ASSERT_EQ(5, b.p);
  ASSERT_EQ(0, byte_diff(b.x, 5, "12345"));
}

/*
 * int buffer_put8long(buffer*, unsigned long);
 */
TEST(test_buffer_put8long) {
  OUTBUF(b);

  buffer_put8long(&b, 8);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "10"));
}

/*
 * int buffer_putxlong(buffer*, unsigned long);
 */
TEST(test_buffer_putxlong) {
  OUTBUF(b);

  buffer_putxlong(&b, 255);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "ff"));
}

/*
 * int buffer_putlong(buffer*, signed long);
 */
TEST(test_buffer_putlong) {
  OUTBUF(b);

  buffer_putlong(&b, -42);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "-42"));
}

/*
 * int buffer_putdouble(buffer*, double, int);
 */
TEST(test_buffer_putdouble) {
  buffer b;
  char mem[64];

  buffer_init(&b, (buffer_op_ptr)0, -1, mem, sizeof mem);
  ASSERT_EQ(0, buffer_putdouble(&b, 3.14159, 2));
  ASSERT_EQ(0, byte_diff(mem, b.p, "3.14"));
  b.p = 0;
  buffer_putdouble(&b, -0.5, 3);
  ASSERT_EQ(0, byte_diff(mem, b.p, "-0.5"));
}

/*
 * int buffer_puterror(buffer*);
 */
TEST(test_buffer_puterror) {
  buffer b;
  char mem[128];

  buffer_init(&b, (buffer_op_ptr)0, -1, mem, sizeof mem);
  errno = ENOENT;
  buffer_puterror(&b);
  ASSERT_EQ(1, b.p > 0);
  ASSERT_EQ(0, byte_diff(mem, b.p, strerror(ENOENT)));
}

/*
 * int buffer_puterror2(buffer*, int);
 */
TEST(test_buffer_puterror2) {
  OUTBUF(b);

  buffer_puterror2(&b, 0);
  ASSERT_NE(0, b.p);
}

/*
 * int buffer_putsa(buffer*, const stralloc* sa);
 */
TEST(test_buffer_putsa) {
  OUTBUF(b);
  stralloc sa;

  stralloc_init(&sa);
  stralloc_copys(&sa, "hello");
  buffer_putsa(&b, &sa);

  ASSERT_EQ(5, b.p);
  ASSERT_EQ(0, byte_diff(b.x, 5, "hello"));

  stralloc_free(&sa);
}

/*
 * int buffer_putsaflush(buffer*, const stralloc* sa);
 */
TEST(test_buffer_putsaflush) {
  buffer b;
  char mem[32], t[16];
  stralloc sa;
  int fd = open_trunc(TESTFILE);

  stralloc_init(&sa);
  stralloc_copys(&sa, "sa data");
  buffer_init(&b, (buffer_op_ptr)write, fd, mem, sizeof mem);
  ASSERT_EQ(0, buffer_putsaflush(&b, &sa));
  close(fd);
  ASSERT_EQ(7, read_testfile(t, 16));
  stralloc_free(&sa);
}

/*
 * int buffer_get_token_sa(buffer*, stralloc* sa, const char* charset, size_t setlen);
 */
TEST(test_buffer_get_token_sa) {
  buffer in;
  stralloc sa;

  stralloc_init(&sa);
  buffer_frombuf(&in, "foo,bar", 7);
  buffer_get_token_sa(&in, &sa, ",", 1);

  ASSERT_SA_EQUALS(&sa, "foo,");

  stralloc_free(&sa);
}

/*
 * int buffer_getline_sa(buffer*, stralloc* sa);
 */
TEST(test_buffer_getline_sa) {
  buffer in;
  stralloc sa;

  stralloc_init(&sa);
  buffer_frombuf(&in, "line1\nline2", 11);
  buffer_getline_sa(&in, &sa);

  ASSERT_SA_EQUALS(&sa, "line1\n");

  stralloc_free(&sa);
}

/*
 * int buffer_get_new_token_sa(buffer*, stralloc* sa, const char* charset, size_t setlen);
 */
TEST(test_buffer_get_new_token_sa) {
  buffer in;
  stralloc sa;

  stralloc_init(&sa);
  stralloc_copys(&sa, "leftover");
  buffer_frombuf(&in, "foo,bar", 7);
  buffer_get_new_token_sa(&in, &sa, ",", 1);

  ASSERT_SA_EQUALS(&sa, "foo,");

  stralloc_free(&sa);
}

/*
 * int buffer_getnewline_sa(buffer*, stralloc* sa);
 */
TEST(test_buffer_getnewline_sa) {
  buffer in;
  stralloc sa;

  stralloc_init(&sa);
  stralloc_copys(&sa, "leftover");
  buffer_frombuf(&in, "line1\nline2", 11);
  buffer_getnewline_sa(&in, &sa);

  ASSERT_SA_EQUALS(&sa, "line1\n");

  stralloc_free(&sa);
}

/*
 * typedef int (*sa_predicate)(stralloc* sa, void*);
 * int buffer_get_token_sa_pred(buffer*, stralloc* sa, sa_predicate p, void*);
 * int buffer_get_new_token_sa_pred(buffer*, stralloc* sa, sa_predicate p, void*);
 */
TEST(test_buffer_get_token_sa_pred) {
  buffer b;
  stralloc sa;
  char term = ',';
  char text[] = "one;two,three";

  stralloc_init(&sa);
  buffer_fromstr(&b, text, sizeof(text) - 1);
  ASSERT_EQ(0, buffer_get_token_sa_pred(&b, &sa, sp_end_semicolon));
  ASSERT_EQ(4, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s, 4, "one;"));
  stralloc_zero(&sa);
  ASSERT_EQ(0, buffer_get_token_sa_pred_arg(&b, &sa, sp_end_arg, &term));
  ASSERT_EQ(4, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s, 4, "two,"));
  stralloc_free(&sa);
}
TEST(test_buffer_get_new_token_sa_pred) {
  buffer b;
  stralloc sa;
  char term = ',';
  char text[] = "a;b,";

  stralloc_init(&sa);
  stralloc_copys(&sa, "garbage");
  buffer_fromstr(&b, text, sizeof(text) - 1);
  ASSERT_EQ(0, buffer_get_new_token_sa_pred(&b, &sa, sp_end_semicolon));
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "a;")); /* emptied first */
  ASSERT_EQ(2, sa.len);
  stralloc_copys(&sa, "more garbage");
  ASSERT_EQ(0, buffer_get_new_token_sa_pred_arg(&b, &sa, sp_end_arg, &term));
  ASSERT_EQ(2, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s, 2, "b,"));
  stralloc_free(&sa);
}

/*
 * void buffer_fromsa(buffer*, const stralloc* sa);
 */
TEST(test_buffer_fromsa) {
  buffer in;
  stralloc sa;
  char out[8] = {0};

  stralloc_init(&sa);
  stralloc_copys(&sa, "hello");
  buffer_fromsa(&in, &sa);

  ASSERT_EQ(5, buffer_get(&in, out, 5));
  ASSERT_EQ(0, byte_diff(out, 5, "hello"));

  stralloc_free(&sa);
}

/*
 * int buffer_tosa(buffer* b, stralloc* sa);
 *
 * Makes b a write sink that appends to sa (the reverse of
 * buffer_fromsa, which makes b a read source over sa's contents).
 */
TEST(test_buffer_tosa) {
  buffer out;
  stralloc sa;

  stralloc_init(&sa);
  buffer_tosa(&out, &sa);

  buffer_puts(&out, "hello");
  buffer_flush(&out);

  ASSERT_SA_EQUALS(&sa, "hello");

  stralloc_free(&sa);
}

/*
 * int buffer_gettok_sa(buffer*, stralloc* sa, const char* charset, size_t setlen);
 */
TEST(test_buffer_gettok_sa) {
  buffer b;
  stralloc sa;
  char text[] = "key=value";

  stralloc_init(&sa);
  buffer_fromstr(&b, text, sizeof(text) - 1);
  ASSERT_EQ(3, buffer_gettok_sa(&b, &sa, "=:", 2));
  ASSERT_EQ(3, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s, 3, "key"));
  stralloc_free(&sa);
}

/*
 * int buffer_getln(buffer*, stralloc*, int*, int);
 */
TEST(test_buffer_getln) {
  /* covered by test_buffer_getln in test_stralloc.c */
  ASSERT_EQ(1, 1);
}

/*
 * int buffer_put_escaped(buffer*, const void*, size_t, size_t (*escape)(char*, int));
 */
TEST(test_buffer_put_escaped) {
  buffer b;
  stralloc sa;

  stralloc_init(&sa);
  buffer_tosa(&b, &sa);
  ASSERT_EQ(10, buffer_put_escaped(&b, "aXbc", 4, esc_upper_hex));
  buffer_flush(&b);
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "<A>X<B><C>"));
  stralloc_free(&sa);
}

/*
 * int buffer_putsa_escaped(buffer*, const stralloc* sa, size_t (*escape)(char*, int));
 */
TEST(test_buffer_putsa_escaped) {
  buffer b;
  stralloc sa, in;

  stralloc_init(&sa);
  stralloc_init(&in);
  stralloc_copys(&in, "q1");
  buffer_tosa(&b, &sa);
  ASSERT_EQ(4, buffer_putsa_escaped(&b, &in, esc_upper_hex));
  buffer_flush(&b);
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "<Q>1"));
  stralloc_free(&sa);
  stralloc_free(&in);
}

/*
 * void buffer_frombuf(buffer*, const char* x, size_t l);
 */
TEST(test_buffer_frombuf) {
  buffer in;
  char out[4] = {0};

  buffer_frombuf(&in, "abcd", 4);
  ASSERT_EQ(4, buffer_get(&in, out, 4));
  ASSERT_EQ(0, byte_diff(out, 4, "abcd"));
}

/*
 * int buffer_copybuf(buffer*, const char* x, size_t l);
 */
TEST(test_buffer_copybuf) {
  buffer in;
  char out[4] = {0};

  ASSERT_EQ(0, buffer_copybuf(&in, "wxyz", 4));
  ASSERT_EQ(4, buffer_get(&in, out, 4));
  ASSERT_EQ(0, byte_diff(out, 4, "wxyz"));

  buffer_close(&in);
}

/*
 * void buffer_fromarray(buffer*, array* a);
 */
TEST(test_buffer_fromarray) {
  array a = ARRAY_INIT();
  buffer in;
  char out[3] = {0};

  array_cats(&a, "abc");
  buffer_fromarray(&in, &a);

  ASSERT_EQ(3, buffer_get(&in, out, 3));
  ASSERT_EQ(0, byte_diff(out, 3, "abc"));

  array_reset(&a);
}

/*
 * void buffer_dump(buffer* out, buffer* b);
 */
TEST(test_buffer_dump) {
  buffer in, out;
  char mem[256];

  buffer_frombuf(&in, "ab\x01 z", 5);
  buffer_init(&out, (buffer_op_ptr)discard_write, -1, mem, sizeof mem);
  buffer_dump(&out, &in); /* ends with a flush, which the discarding writer swallows */
  ASSERT_EQ(0, out.p);
}

/*
 * int buffer_putc(buffer*, char c);
 */
TEST(test_buffer_putc) {
  OUTBUF(b);

  buffer_putc(&b, 'x');
  ASSERT_EQ(1, b.p);
  ASSERT_EQ('x', b.x[0]);
}

/*
 * int buffer_putnspace(buffer*, int n);
 */
TEST(test_buffer_putnspace) {
  OUTBUF(b);

  buffer_putnspace(&b, 3);
  ASSERT_EQ(3, b.p);
  ASSERT_EQ(0, byte_diff(b.x, 3, "   "));
}

/*
 * int buffer_putptr(buffer*, const void* ptr);
 */
TEST(test_buffer_putptr) {
  OUTBUF(b);

  buffer_putptr(&b, NULL);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "(null)"));
}

/*
 * int buffer_putulong0(buffer*, unsigned long l, int pad);
 */
TEST(test_buffer_putulong0) {
  OUTBUF(b);

  buffer_putulong0(&b, 5, 3);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "  5"));
}

/*
 * int buffer_putlong0(buffer*, long l, int pad);
 */
TEST(test_buffer_putlong0) {
  OUTBUF(b);

  buffer_putlong0(&b, -5, 4);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "  -5"));
}

/*
 * int buffer_putxlong0(buffer*, unsigned long l, int pad);
 */
TEST(test_buffer_putxlong0) {
  OUTBUF(b);

  buffer_putxlong0(&b, 0xf, 4);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "000f"));
}

/*
 * int buffer_skipspace(buffer* b);
 */
TEST(test_buffer_skipspace) {
  buffer in;

  /* the run of spaces is skipped and the byte after it stays */
  buffer_frombuf(&in, "   x", 4);
  ASSERT_EQ(3, buffer_skipspace(&in));
  ASSERT_EQ('x', *buffer_peek(&in));
  ASSERT_EQ(0, buffer_skipspace(&in));
  ASSERT_EQ(1, buffer_skipc(&in));
  ASSERT_EQ(1, buffer_EOF(&in));
}

/*
 * int buffer_skip_pred(buffer*, int (*pred)(int));
 */
TEST(test_buffer_skip_pred) {
  buffer b;
  char text[] = "   \t word";
  char c;

  buffer_fromstr(&b, text, sizeof(text) - 1);
  ASSERT_EQ(5, buffer_skip_pred(&b, isspace));
  ASSERT_EQ(1, buffer_getc(&b, &c));
  ASSERT_EQ('w', c);
  ASSERT_EQ(0, buffer_skip_pred(&b, isspace)); /* already in front of a non-space */
}

/*
 * int buffer_putfmt_args(buffer*, const char* x, size_t len, size_t (*escape)(...), void* args[]);
 * int buffer_putfmt(buffer* b, const char* x, size_t len, size_t (*escape)(char*, int), ...);
 */
TEST(test_buffer_putfmt_args) {
  buffer b;
  stralloc sa;
  char x = '1', y = '2';
  void* args[4];

  args[0] = &x;
  args[1] = &y;
  args[2] = args[3] = NULL;
  stralloc_init(&sa);
  buffer_tosa(&b, &sa);
  ASSERT_EQ(6, buffer_putfmt_args(&b, "ab", 2, (size_t (*)(char*, int, void*, void*, void*, void*))esc_with_arg, args));
  buffer_flush(&b);
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "a12b12"));
  stralloc_free(&sa);
}
TEST(test_buffer_putfmt) {
  buffer b;
  stralloc sa;

  stralloc_init(&sa);
  buffer_tosa(&b, &sa);
  ASSERT_EQ(7, buffer_putfmt(&b, "a-b", 3, esc_upper_hex, NULL, NULL, NULL, NULL));
  buffer_flush(&b);
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "<A>-<B>"));
  stralloc_free(&sa);
}

/*
 * int buffer_puts_escaped(buffer* b, const char* s, size_t (*escape)(char*, int));
 */
TEST(test_buffer_puts_escaped) {
  buffer b;
  stralloc sa;

  stralloc_init(&sa);
  buffer_tosa(&b, &sa);
  ASSERT_EQ(7, buffer_puts_escaped(&b, "a-b", esc_upper_hex));
  buffer_flush(&b);
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "<A>-<B>"));
  stralloc_free(&sa);
}

/*
 * int buffer_put_quoted(buffer* b, const char* x, size_t len);
 */
TEST(test_buffer_put_quoted) {
  buffer b;
  stralloc sa;

  stralloc_init(&sa);
  buffer_tosa(&b, &sa);
  buffer_put_quoted(&b, "a\nb", 3);
  buffer_flush(&b);
  /* a control character must not vanish into its digits: it is written as a backslash and octal */
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "a\\012b"));
  stralloc_free(&sa);
}

/*
 * ssize_t buffer_feed(buffer* b);
 */
TEST(test_buffer_feed) {
  buffer in;

  buffer_frombuf(&in, "abc", 3);
  ASSERT_EQ(3, buffer_feed(&in));
}

/*
 * ssize_t buffer_freshen(buffer* b);
 */
TEST(test_buffer_freshen) {
  buffer in;

  buffer_frombuf(&in, "abc def", 7);
  ASSERT_EQ(7, buffer_freshen(&in));
}

/*
 * ssize_t buffer_prefetch(buffer*, size_t n);
 */
TEST(test_buffer_prefetch) {
  buffer in;

  buffer_frombuf(&in, "abc def", 7);
  ASSERT_EQ(7, buffer_prefetch(&in, 3));
  ASSERT_EQ(0, in.p); /* nothing consumed */
}

/*
 * int buffer_appendfile(buffer* b, const char* fn);
 * int buffer_readfile(buffer* b, const char* fn);
 * int buffer_truncfile(buffer* b, const char* fn);
 * int buffer_writefile(buffer* b, const char* fn);
 */
TEST(test_buffer_appendfile) {
  buffer b;
  char t[16];

  write_testfile("AB");
  ASSERT_EQ(0, buffer_appendfile(&b, TESTFILE));
  buffer_puts(&b, "CD");
  buffer_flush(&b);
  buffer_free(&b);
  ASSERT_EQ(4, read_testfile(t, 16));
  ASSERT_EQ(0, byte_diff(t, 4, "ABCD"));
}
TEST(test_buffer_readfile) {
  buffer b;
  char out[16];

  write_testfile("file data");
  ASSERT_EQ(0, buffer_readfile(&b, TESTFILE));
  ASSERT_EQ(9, buffer_get(&b, out, 16));
  ASSERT_EQ(0, byte_diff(out, 9, "file data"));
  buffer_free(&b);
  ASSERT_EQ(-1, buffer_readfile(&b, "/nonexistent/dir/file"));
}
TEST(test_buffer_truncfile) {
  buffer b;
  char t[16];

  write_testfile("long old content");
  ASSERT_EQ(0, buffer_truncfile(&b, TESTFILE));
  buffer_puts(&b, "new");
  buffer_flush(&b);
  buffer_free(&b);
  ASSERT_EQ(3, read_testfile(t, 16));
  ASSERT_EQ(0, byte_diff(t, 3, "new"));
}
TEST(test_buffer_writefile) {
  buffer b;
  char t[16];

  write_testfile("ABCD");
  ASSERT_EQ(0, buffer_writefile(&b, TESTFILE)); /* no truncation: overwrites from the start */
  buffer_puts(&b, "Z");
  buffer_flush(&b);
  buffer_free(&b);
  ASSERT_EQ(4, read_testfile(t, 16));
  ASSERT_EQ(0, byte_diff(t, 4, "ZBCD"));
}

/*
 * int buffer_lzma(buffer*, buffer*, int compress);
 * int buffer_bz2(buffer*, buffer*, int compress);
 * int buffer_brotli(buffer*, buffer*, int compress);
 */
TEST(test_buffer_lzma) {
#if HAVE_LIBLZMA
  buffer file, xz, in, unxz;
  static char data[50000], out[50000];
  size_t i, got = 0;
  ssize_t n;

  for(i = 0; i < sizeof(data); i++)
    data[i] = (char)('a' + (i * 5 + i / 11) % 26);

  ASSERT_EQ(0, buffer_truncfile(&file, "test_buffer_lzma.tmp"));
  ASSERT_NE(0, buffer_lzma(&xz, &file, 1));
  buffer_put(&xz, data, sizeof(data));
  buffer_flush(&xz);
  buffer_close(&xz);
  buffer_close(&file);

  ASSERT_EQ(0, buffer_mmapprivate(&in, "test_buffer_lzma.tmp"));
  ASSERT_NE(0, buffer_lzma(&unxz, &in, 0));
  while((n = buffer_get(&unxz, out + got, sizeof(out) - got)) > 0)
    got += (size_t)n;
  unlink("test_buffer_lzma.tmp");

  ASSERT_EQ(sizeof(data), got);
  ASSERT_EQ(0, byte_diff(out, got, data));
#endif
}
TEST(test_buffer_bz2) {
#if HAVE_LIBBZ2
  buffer file, bz, in, unbz;
  static char data[400000], out[400000];
  size_t i, got = 0;
  ssize_t n;

  /* more than one 300000 byte bzip2 block at level 3 */
  for(i = 0; i < sizeof(data); i++)
    data[i] = (char)('a' + (i * 7 + i / 13) % 26);

  ASSERT_EQ(0, buffer_truncfile(&file, "test_buffer_bz2.tmp"));
  ASSERT_NE(0, buffer_bz2(&bz, &file, 3));
  buffer_put(&bz, data, sizeof(data));
  buffer_flush(&bz);
  buffer_close(&bz);
  buffer_close(&file);

  ASSERT_EQ(0, buffer_mmapprivate(&in, "test_buffer_bz2.tmp"));
  ASSERT_NE(0, buffer_bz2(&unbz, &in, 0));
  while((n = buffer_get(&unbz, out + got, sizeof(out) - got)) > 0)
    got += (size_t)n;
  unlink("test_buffer_bz2.tmp");

  ASSERT_EQ(sizeof(data), got);
  ASSERT_EQ(0, byte_diff(out, got, data));
#endif
}
TEST(test_buffer_brotli) {
#if HAVE_BROTLI
  buffer file, br, in, unbr;
  static char data[50000], out[50000];
  size_t i, got = 0;
  ssize_t n;

  for(i = 0; i < sizeof(data); i++)
    data[i] = (char)('a' + (i * 3 + i / 7) % 26);

  ASSERT_EQ(0, buffer_truncfile(&file, "test_buffer_brotli.tmp"));
  ASSERT_NE(0, buffer_brotli(&br, &file, 5));
  buffer_put(&br, data, sizeof(data));
  buffer_flush(&br);
  buffer_close(&br);
  buffer_close(&file);

  ASSERT_EQ(0, buffer_mmapprivate(&in, "test_buffer_brotli.tmp"));
  ASSERT_NE(0, buffer_brotli(&unbr, &in, 0));
  while((n = buffer_get(&unbr, out + got, sizeof(out) - got)) > 0)
    got += (size_t)n;
  unlink("test_buffer_brotli.tmp");

  ASSERT_EQ(sizeof(data), got);
  ASSERT_EQ(0, byte_diff(out, got, data));
#endif
}

/*
 * int buffer_putnc(buffer*, char c, int ntimes);
 */
TEST(test_buffer_putnc) {
  OUTBUF(b);

  buffer_putnc(&b, 'z', 3);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "zzz"));
}

/*
 * int buffer_putns(buffer*, const char* s, int ntimes);
 */
TEST(test_buffer_putns) {
  OUTBUF(b);

  buffer_putns(&b, "ab", 3);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "ababab"));
}

/*
 * int buffer_putspad(buffer*, const char* x, size_t pad);
 */
TEST(test_buffer_putspad) {
  OUTBUF(b);

  buffer_putspad(&b, "hi", 4);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "hi  "));
}

/*
 * int buffer_putspadstart(buffer*, const char* x, size_t pad);
 */
TEST(test_buffer_putspadstart) {
  OUTBUF(b);

  buffer_putspadstart(&b, "hi", 4);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "  hi"));
}

/*
 * int buffer_deflate(buffer*, buffer* out, int level);
 * int buffer_inflate(buffer*, buffer* in);
 */
TEST(test_buffer_deflate) {
#if HAVE_ZLIB
  buffer file, deflate, in, inflate;
  static char data[400000], out[400000];
  size_t i, got = 0;
  ssize_t n;

  ASSERT_EQ(0, buffer_truncfile(&file, "test_buffer_zip.tmp"));
  ASSERT_NE(0, buffer_deflate(&deflate, &file, 9));
  buffer_puts(&deflate, "gzipp'd test text\n");
  buffer_flush(&deflate);
  buffer_close(&deflate);
  buffer_close(&file);

  /* round trip: inflating what was deflated gives the text back */
  ASSERT_EQ(0, buffer_mmapprivate(&in, "test_buffer_zip.tmp"));
  ASSERT_NE(0, buffer_inflate(&inflate, &in));
  n = buffer_get(&inflate, out, sizeof(out));
  buffer_close(&inflate);

  ASSERT_EQ(18, n);
  ASSERT_EQ(0, byte_diff(out, 18, "gzipp'd test text\n"));

  /* well over the 32768 bytes of one deflate window and the buffers */
  for(i = 0; i < sizeof(data); i++)
    data[i] = (char)('a' + (i * 7 + i / 13) % 26);

  ASSERT_EQ(0, buffer_truncfile(&file, "test_buffer_zip.tmp"));
  ASSERT_NE(0, buffer_deflate(&deflate, &file, 6));
  buffer_put(&deflate, data, sizeof(data));
  buffer_flush(&deflate);
  buffer_close(&deflate);
  buffer_close(&file);

  ASSERT_EQ(0, buffer_mmapprivate(&in, "test_buffer_zip.tmp"));
  ASSERT_NE(0, buffer_inflate(&inflate, &in));
  while((n = buffer_get(&inflate, out + got, sizeof(out) - got)) > 0)
    got += (size_t)n;
  buffer_close(&inflate);
  unlink("test_buffer_zip.tmp");

  ASSERT_EQ(sizeof(data), got);
  ASSERT_EQ(0, byte_diff(out, got, data));
#endif
}
TEST(test_buffer_inflate) {
#if HAVE_ZLIB
  buffer file, deflate, in, inflate;
  static char data[100000], out[100000];
  size_t i, len, got = 0;
  ssize_t n;

  for(i = 0; i < sizeof(data); i++)
    data[i] = (char)('a' + (i * 7 + i / 13) % 26);

  ASSERT_EQ(0, buffer_truncfile(&file, "test_buffer_inflate.tmp"));
  ASSERT_NE(0, buffer_deflate(&deflate, &file, 6));
  buffer_put(&deflate, data, sizeof(data));
  buffer_flush(&deflate);
  buffer_close(&deflate);
  len = (size_t)lseek(file.fd, 0, SEEK_CUR);
  buffer_close(&file);

  /* the file cut short is an error, not the end of the data */
  ASSERT_EQ(0, truncate("test_buffer_inflate.tmp", (off_t)(len / 2)));
  ASSERT_EQ(0, buffer_mmapprivate(&in, "test_buffer_inflate.tmp"));
  ASSERT_NE(0, buffer_inflate(&inflate, &in));
  while((n = buffer_get(&inflate, out + got, sizeof(out) - got)) > 0)
    got += (size_t)n;
  buffer_close(&inflate);
  unlink("test_buffer_inflate.tmp");

  ASSERT_EQ(-1, n);
#endif
}

/*
 * int buffer_gunzip(buffer*, const char* filename);
 * int buffer_gunzip_fd(buffer*, fd_type fd);
 * int buffer_gzip(buffer*, const char* filename, int level);
 * int buffer_gzip_fd(buffer*, fd_type fd, int level);
 * int buffer_bunzip(buffer*, const char* filename);
 * int buffer_bunzip_fd(buffer*, fd_type fd);
 * int buffer_bzip(buffer*, const char* filename, int level);
 * int buffer_bzip_fd(buffer*, fd_type fd, int level);
 */
TEST(test_buffer_gunzip) {
  buffer b;
  char out[32];

  /* zlib's gzread passes data that is not gzip through unchanged */
  write_testfile("plain text, not gzip");
  ASSERT_EQ(0, buffer_gunzip(&b, TESTFILE));
  ASSERT_EQ(20, buffer_get(&b, out, 32));
  ASSERT_EQ(0, byte_diff(out, 20, "plain text, not gzip"));
  buffer_close(&b);
}
TEST(test_buffer_gunzip_fd) {
  buffer b;
  int fd;
  char out[64];

  ASSERT_EQ(0, buffer_gzip(&b, TESTFILE ".gz", 9));
  buffer_puts(&b, "via fd");
  buffer_flush(&b);
  buffer_close(&b);
  fd = open_read(TESTFILE ".gz");
  ASSERT_EQ(0, buffer_gunzip_fd(&b, fd));
  ASSERT_EQ(6, buffer_get(&b, out, 64));
  ASSERT_EQ(0, byte_diff(out, 6, "via fd"));
  buffer_close(&b);
  close(fd);
  unlink(TESTFILE ".gz");
}
TEST(test_buffer_gzip) {
  buffer b;
  char out[64];
  ssize_t n;

  ASSERT_EQ(0, buffer_gzip(&b, TESTFILE ".gz", 6));
  buffer_puts(&b, "compress me compress me compress me");
  buffer_flush(&b);
  buffer_close(&b);
  ASSERT_EQ(0, buffer_gunzip(&b, TESTFILE ".gz"));
  n = buffer_get(&b, out, 64);
  ASSERT_EQ(35, n);
  ASSERT_EQ(0, byte_diff(out, 35, "compress me compress me compress me"));
  buffer_close(&b);
  unlink(TESTFILE ".gz");
}
TEST(test_buffer_gzip_fd) {
  buffer b;
  int fd = open_trunc(TESTFILE ".gz");
  char out[64];

  ASSERT_EQ(0, buffer_gzip_fd(&b, fd, 1));
  buffer_puts(&b, "level one");
  buffer_flush(&b);
  buffer_close(&b);
  close(fd);
  ASSERT_EQ(0, buffer_gunzip(&b, TESTFILE ".gz"));
  ASSERT_EQ(9, buffer_get(&b, out, 64));
  buffer_close(&b);
  unlink(TESTFILE ".gz");
}
TEST(test_buffer_bunzip) {
  /* decompression is checked together with buffer_bzip */
  ASSERT_EQ(1, 1);
}
TEST(test_buffer_bunzip_fd) {
  /* decompression is checked together with buffer_bzip_fd */
  ASSERT_EQ(1, 1);
}
TEST(test_buffer_bzip) {
#if HAVE_LIBBZ2
  buffer b;
  char out[64];

  ASSERT_EQ(0, buffer_bzip(&b, TESTFILE ".bz2", 9));
  buffer_puts(&b, "bzip me");
  buffer_flush(&b);
  buffer_close(&b);
  ASSERT_EQ(0, buffer_bunzip(&b, TESTFILE ".bz2"));
  ASSERT_EQ(7, buffer_get(&b, out, sizeof out));
  ASSERT_EQ(0, byte_diff(out, 7, "bzip me"));
  buffer_close(&b);
  ASSERT_EQ(-1, buffer_bunzip(&b, "/nonexistent-dir/x.bz2"));
  unlink(TESTFILE ".bz2");
#endif
}
TEST(test_buffer_bzip_fd) {
#if HAVE_LIBBZ2
  buffer b;
  char out[64];
  int fd = open_trunc(TESTFILE ".bz2");

  ASSERT_EQ(0, buffer_bzip_fd(&b, fd, 1));
  buffer_puts(&b, "level one");
  buffer_flush(&b);
  buffer_close(&b);
  fd = open_read(TESTFILE ".bz2");
  ASSERT_EQ(0, buffer_bunzip_fd(&b, fd));
  ASSERT_EQ(9, buffer_get(&b, out, sizeof out));
  ASSERT_EQ(0, byte_diff(out, 9, "level one"));
  buffer_close(&b);
  unlink(TESTFILE ".bz2");
#endif
}

/*
 * int buffer_get_until(buffer*, char* x, size_t len, const char* charset, size_t setlen);
 */
TEST(test_buffer_get_until) {
  buffer in;
  char out[8] = {0};

  buffer_frombuf(&in, "foo,bar", 7);
  ASSERT_EQ(4, buffer_get_until(&in, out, sizeof(out), ",", 1));
  ASSERT_EQ(0, byte_diff(out, 4, "foo,"));
}

/*
 * int buffer_write_fd(buffer*, fd_type fd);
 */
TEST(test_buffer_write_fd) {
  buffer b;
  char t[16];
  int fd = open_trunc(TESTFILE);

  ASSERT_EQ(0, buffer_write_fd(&b, fd));
  buffer_puts(&b, "written");
  ASSERT_EQ(0, buffer_flush(&b));
  buffer_free(&b);
  close(fd);
  ASSERT_EQ(7, read_testfile(t, 16));
  ASSERT_EQ(0, byte_diff(t, 7, "written"));
}

/*
 * int buffer_putlonglong(buffer*, int64 l);
 * int buffer_putulonglong(buffer*, uint64 l);
 * int buffer_putxlonglong(buffer*, uint64 l);
 * int buffer_putxlonglong0(buffer*, uint64 l, int pad);
 * int buffer_putulonglongpad(buffer*, uint64, int pad);
 */
TEST(test_buffer_putlonglong) {
  OUTBUF(b);

  buffer_putlonglong(&b, -123);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "-123"));
}

TEST(test_buffer_putulonglong) {
  OUTBUF(b);

  buffer_putulonglong(&b, 123);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "123"));
}

TEST(test_buffer_putxlonglong) {
  OUTBUF(b);

  buffer_putxlonglong(&b, 255);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "ff"));
}

TEST(test_buffer_putxlonglong0) {
  OUTBUF(b);

  buffer_putxlonglong0(&b, 0xf, 4);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "000f"));
}

TEST(test_buffer_putulonglongpad) {
  OUTBUF(b);

  buffer_putulonglongpad(&b, 5, 3);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "  5"));
}

/*
 * int buffer_puttai(buffer*, const struct tai*);
 */
TEST(test_buffer_puttai) {
  buffer b;
  char mem[64];
  struct tai t;

  buffer_init(&b, (buffer_op_ptr)0, -1, mem, sizeof mem);
  tai_unix(&t, 0);
  buffer_puttai(&b, &t);
  ASSERT_EQ(20, b.p);
  ASSERT_EQ(0, byte_diff(mem, 20, "1970-01-01T00:00:10Z")); /* TAI, 10 s ahead */
}

/*
 * int buffer_copy(buffer* out, buffer* in);
 */
TEST(test_buffer_copy) {
  buffer in;
  OUTBUF(out);

  buffer_frombuf(&in, "hello", 5);
  ASSERT_EQ(0, buffer_copy(&out, &in));
  ASSERT_EQ(5, out.p);
  ASSERT_EQ(0, byte_diff(out.x, 5, "hello"));
}

/*
 * void buffer_putstr(buffer* b, const char* x);
 */
TEST(test_buffer_putstr) {
  OUTBUF(b);

  buffer_putstr(&b, "hi");
  ASSERT_EQ(0, byte_diff(b.x, b.p, "\"hi\""));
}

/*
 * void buffer_realloc(buffer* b, size_t ylen);
 */
TEST(test_buffer_realloc) {
  char* x = alloc(4);
  buffer b;

  buffer_init(&b, 0, -1, x, 4);
  buffer_realloc(&b, 64);
  ASSERT_EQ(64, b.a);

  buffer_free(&b);
}

/*
 * int buffer_putxlong0u(buffer*, unsigned long l, int pad);
 */
TEST(test_buffer_putxlong0u) {
  OUTBUF(b);

  /* the "u" is for uppercase (fmt_xlongu), not "unsigned". */
  buffer_putxlong0u(&b, 0xf, 4);
  ASSERT_EQ(0, byte_diff(b.x, b.p, "000F"));
}

/*
 * buffer_LINE / buffer_COLUMN
 */
TEST(test_buffer_line_column) {
  buffer in;

  buffer_frombuf(&in, "foo\nbar", 7);
  buffer_SKIP(&in, 7);
  ASSERT_EQ(1, buffer_LINE(&in));
}

#define RUN_BUFFER_TESTS() \
  RUN(test_buffer_init); \
  RUN(test_buffer_init_free); \
  RUN(test_buffer_free); \
  RUN(test_buffer_munmap); \
  RUN(test_buffer_mmapread); \
  RUN(test_buffer_mmapread_fd); \
  RUN(test_buffer_mmapprivate); \
  RUN(test_buffer_mmapprivate_fd); \
  RUN(test_buffer_mmapshared); \
  RUN(test_buffer_mmapshared_fd); \
  RUN(test_buffer_close); \
  RUN(test_buffer_read_fd); \
  RUN(test_buffer_flush); \
  RUN(test_buffer_put); \
  RUN(test_buffer_putalign); \
  RUN(test_buffer_putflush); \
  RUN(test_buffer_puts); \
  RUN(test_buffer_putsalign); \
  RUN(test_buffer_putsflush); \
  RUN(test_buffer_putm_internal); \
  RUN(test_buffer_putm_internal_flush); \
  RUN(test_buffer_putspace); \
  RUN(test_buffer_putnlflush); \
  RUN(test_buffer_get); \
  RUN(test_buffer_getc); \
  RUN(test_buffer_getn); \
  RUN(test_buffer_get_token); \
  RUN(test_buffer_getline); \
  RUN(test_buffer_skip_until); \
  RUN(test_buffer_get_token_pred); \
  RUN(test_buffer_peek); \
  RUN(test_buffer_peekc); \
  RUN(test_buffer_seek); \
  RUN(test_buffer_rewind); \
  RUN(test_buffer_skipc); \
  RUN(test_buffer_skipn); \
  RUN(test_buffer_macros); \
  RUN(test_buffer_GETC); \
  RUN(test_buffer_region_macros); \
  RUN(test_buffer_move_scan); \
  RUN(test_buffer_putulong); \
  RUN(test_buffer_put8long); \
  RUN(test_buffer_putxlong); \
  RUN(test_buffer_putlong); \
  RUN(test_buffer_putdouble); \
  RUN(test_buffer_puterror); \
  RUN(test_buffer_puterror2); \
  RUN(test_buffer_putsa); \
  RUN(test_buffer_putsaflush); \
  RUN(test_buffer_get_token_sa); \
  RUN(test_buffer_getline_sa); \
  RUN(test_buffer_get_new_token_sa); \
  RUN(test_buffer_getnewline_sa); \
  RUN(test_buffer_get_token_sa_pred); \
  RUN(test_buffer_get_new_token_sa_pred); \
  RUN(test_buffer_fromsa); \
  RUN(test_buffer_tosa); \
  RUN(test_buffer_gettok_sa); \
  RUN(test_buffer_getln); \
  RUN(test_buffer_put_escaped); \
  RUN(test_buffer_putsa_escaped); \
  RUN(test_buffer_frombuf); \
  RUN(test_buffer_copybuf); \
  RUN(test_buffer_fromarray); \
  RUN(test_buffer_dump); \
  RUN(test_buffer_putc); \
  RUN(test_buffer_putnspace); \
  RUN(test_buffer_putptr); \
  RUN(test_buffer_putulong0); \
  RUN(test_buffer_putlong0); \
  RUN(test_buffer_putxlong0); \
  RUN(test_buffer_skipspace); \
  RUN(test_buffer_skip_pred); \
  RUN(test_buffer_putfmt_args); \
  RUN(test_buffer_putfmt); \
  RUN(test_buffer_puts_escaped); \
  RUN(test_buffer_put_quoted); \
  RUN(test_buffer_feed); \
  RUN(test_buffer_freshen); \
  RUN(test_buffer_prefetch); \
  RUN(test_buffer_appendfile); \
  RUN(test_buffer_readfile); \
  RUN(test_buffer_truncfile); \
  RUN(test_buffer_writefile); \
  RUN(test_buffer_lzma); \
  RUN(test_buffer_bz2); \
  RUN(test_buffer_brotli); \
  RUN(test_buffer_putnc); \
  RUN(test_buffer_putns); \
  RUN(test_buffer_putspad); \
  RUN(test_buffer_putspadstart); \
  RUN(test_buffer_deflate); \
  RUN(test_buffer_inflate); \
  RUN(test_buffer_gunzip); \
  RUN(test_buffer_gunzip_fd); \
  RUN(test_buffer_gzip); \
  RUN(test_buffer_gzip_fd); \
  RUN(test_buffer_bunzip); \
  RUN(test_buffer_bunzip_fd); \
  RUN(test_buffer_bzip); \
  RUN(test_buffer_bzip_fd); \
  RUN(test_buffer_get_until); \
  RUN(test_buffer_write_fd); \
  RUN(test_buffer_putlonglong); \
  RUN(test_buffer_putulonglong); \
  RUN(test_buffer_putxlonglong); \
  RUN(test_buffer_putxlonglong0); \
  RUN(test_buffer_putulonglongpad); \
  RUN(test_buffer_puttai); \
  RUN(test_buffer_copy); \
  RUN(test_buffer_putstr); \
  RUN(test_buffer_realloc); \
  RUN(test_buffer_putxlong0u); \
  RUN(test_buffer_line_column)

TESTS(buffer) { RUN_BUFFER_TESTS(); }
