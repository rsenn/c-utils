#include "unit_test.h"
#include "../lib/charbuf.h"
#include "../lib/stralloc.h"
#include <ctype.h>
#include <errno.h>

/* a read function that hands out at most one byte per call, to exercise the
 * refill path of the buffer */
struct slow {
  const char* s;
  size_t calls;
};

static ssize_t
slow_read(int fd, void* buf, size_t n, void* b) {
  struct slow* sl = ((charbuf*)b)->ptr;
  (void)fd;
  sl->calls++;
  if(!*sl->s || n == 0)
    return 0;
  *(char*)buf = *sl->s++;
  return 1;
}

static int
is_digit_pred(int c, size_t pos, void* ptr) {
  (void)pos;
  (void)ptr;
  return isdigit(c);
}

/*
 * void charbuf_froms(charbuf*, char* s, unsigned lookahead);
 * ssize_t charbuf_get(charbuf*);  ssize_t charbuf_peek(charbuf*);
 * void charbuf_free(charbuf*);
 */
TEST(test_charbuf_get_peek) {
  charbuf b;

  charbuf_froms(&b, "abc", 8);
  ASSERT_EQ('a', charbuf_peek(&b));
  ASSERT_EQ('a', charbuf_peek(&b)); /* peek does not consume */
  ASSERT_EQ('a', charbuf_get(&b));
  ASSERT_EQ('b', charbuf_get(&b));
  ASSERT_EQ('c', charbuf_get(&b));
  ASSERT_EQ(0, charbuf_get(&b)); /* end of input */
  ASSERT_EQ(0, charbuf_peek(&b));
  ASSERT_EQ(1, charbuf_done(&b));
  charbuf_free(&b);
}

/*
 * ssize_t charbuf_next / charbuf_getc / charbuf_peekc / charbuf_nextc
 */
TEST(test_charbuf_c_variants) {
  charbuf b;
  unsigned char ch = 0;

  charbuf_froms(&b, "xyz", 8);
  ASSERT_EQ(1, charbuf_peekc(&b, &ch));
  ASSERT_EQ('x', ch);
  ASSERT_EQ(1, charbuf_getc(&b, &ch));
  ASSERT_EQ('x', ch);
  ASSERT_EQ('z', charbuf_next(&b) == 'z' ? 'z' : 0); /* skip y, peek z */
  ASSERT_EQ(1, charbuf_peekc(&b, &ch));
  ASSERT_EQ('z', ch);
  ASSERT_EQ(0, charbuf_nextc(&b, &ch)); /* skip z, then end */
  charbuf_free(&b);
}

/*
 * uint8* charbuf_peekn(charbuf*, unsigned int n);
 */
TEST(test_charbuf_peekn) {
  charbuf b;
  uint8* p;

  charbuf_froms(&b, "hello", 8);
  p = charbuf_peekn(&b, 3);
  ASSERT_EQ(1, p != NULL);
  ASSERT_EQ(0, byte_diff(p, 3, "hel"));
  ASSERT_EQ('h', charbuf_get(&b)); /* peeked bytes are still there */
  p = charbuf_peekn(&b, 4);
  ASSERT_EQ(1, p != NULL);
  ASSERT_EQ(0, byte_diff(p, 4, "ello"));
  ASSERT_EQ(1, charbuf_peekn(&b, 5) == NULL); /* only 4 bytes are left */
  charbuf_free(&b);
}

/*
 * line and column tracking in charbuf.loc
 */
TEST(test_charbuf_location) {
  charbuf b;

  charbuf_froms(&b, "ab\ncd", 8);
  ASSERT_EQ(0, b.loc.line);
  ASSERT_EQ(0, b.loc.column);
  charbuf_get(&b);
  charbuf_get(&b);
  ASSERT_EQ(0, b.loc.line);
  ASSERT_EQ(2, b.loc.column);
  charbuf_get(&b); /* the newline */
  ASSERT_EQ(1, b.loc.line);
  ASSERT_EQ(0, b.loc.column);
  charbuf_get(&b);
  ASSERT_EQ(1, b.loc.column);
  ASSERT_EQ(4, b.offset);
  charbuf_free(&b);
}

/*
 * ssize_t charbuf_skipn(charbuf*, size_t n);
 * ssize_t charbuf_skip_until(charbuf*, int c);
 * ssize_t charbuf_skip_ws(charbuf*);
 */
TEST(test_charbuf_skip) {
  charbuf b;

  charbuf_froms(&b, "abcdef;  \t\n x", 8);
  ASSERT_EQ(2, charbuf_skipn(&b, 2));
  ASSERT_EQ('c', charbuf_peek(&b));
  ASSERT_EQ(4, charbuf_skip_until(&b, ';')); /* skips c d e f */
  ASSERT_EQ(';', charbuf_peek(&b));
  ASSERT_EQ(1, charbuf_skipn(&b, 1));
  ASSERT_EQ(5, charbuf_skip_ws(&b)); /* two spaces, tab, newline, space */
  charbuf_free(&b);

  charbuf_froms(&b, "ab", 8);
  ASSERT_EQ(2, charbuf_skipn(&b, 10)); /* stops at the end of input */
  charbuf_free(&b);
}

/*
 * ssize_t charbuf_skip_ws(charbuf*) must report the characters it skipped even
 * when the input ends right after them.
 */
TEST(test_charbuf_skip_ws_at_eof) {
  charbuf b;

  charbuf_froms(&b, "   ", 8);
  ASSERT_EQ(3, charbuf_skip_ws(&b));
  ASSERT_EQ(0, charbuf_skip_ws(&b));
  charbuf_free(&b);
}

/*
 * ssize_t charbuf_pred_skip / charbuf_pred_lookahead / charbuf_skip_ifeq
 */
TEST(test_charbuf_predicates) {
  charbuf b;

  charbuf_froms(&b, "123abc", 8);
  ASSERT_EQ(3, charbuf_pred_lookahead(&b, is_digit_pred, NULL));
  ASSERT_EQ('1', charbuf_peek(&b)); /* lookahead consumes nothing */
  ASSERT_EQ(3, charbuf_pred_skip(&b, is_digit_pred, NULL));
  ASSERT_EQ('a', charbuf_peek(&b));
  ASSERT_EQ(0, charbuf_pred_skip(&b, is_digit_pred, NULL));
  ASSERT_EQ(1, charbuf_skip_ifeq(&b, 'a'));
  ASSERT_EQ(0, charbuf_skip_ifeq(&b, 'x'));
  ASSERT_EQ(1, charbuf_skip_ifset(&b, "xyb", 3) > 0);
  ASSERT_EQ('c', charbuf_peek(&b));
  charbuf_free(&b);
}

/*
 * ssize_t charbuf_pred_get(charbuf*, stralloc*, pred, ptr);
 */
TEST(test_charbuf_pred_get) {
  charbuf b;
  stralloc sa;

  stralloc_init(&sa);
  charbuf_froms(&b, "42rest", 8);
  ASSERT_EQ(2, charbuf_pred_get(&b, &sa, is_digit_pred, NULL));
  ASSERT_EQ(2, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s, 2, "42"));
  ASSERT_EQ('r', charbuf_peek(&b));
  stralloc_free(&sa);
  charbuf_free(&b);
}

/*
 * a source that delivers a byte per read call still parses correctly
 */
TEST(test_charbuf_slow_source) {
  charbuf b;
  struct slow sl = {"slow source", 0};
  size_t n = 0;

  charbuf_init(&b, slow_read, -1, 8);
  b.ptr = &sl;
  while(charbuf_get(&b) > 0)
    ++n;
  ASSERT_EQ(11, n);
  ASSERT_EQ(1, sl.calls >= 11);
  charbuf_free(&b);
}

/*
 * a failing read function sets the error state
 */
static ssize_t
failing_read(int fd, void* buf, size_t n, void* b) {
  (void)fd;
  (void)buf;
  (void)n;
  (void)b;
  errno = EIO;
  return -1;
}

TEST(test_charbuf_error) {
  charbuf b;

  charbuf_init(&b, failing_read, -1, 8);
  ASSERT_EQ(1, charbuf_get(&b) < 0);
  ASSERT_EQ(1, charbuf_done(&b));
  ASSERT_EQ(1, charbuf_peek(&b) < 0);
  charbuf_free(&b);
}

/*
 * void charbuf_dump(charbuf*, buffer*);  (exists only with buffer.h)
 */
TEST(test_charbuf_free_twice) {
  charbuf b;

  charbuf_froms(&b, "x", 8);
  charbuf_free(&b);
  charbuf_free(&b); /* must not crash */
}

#define RUN_CHARBUF_TESTS() \
  RUN(test_charbuf_get_peek); \
  RUN(test_charbuf_c_variants); \
  RUN(test_charbuf_peekn); \
  RUN(test_charbuf_location); \
  RUN(test_charbuf_skip); \
  RUN(test_charbuf_skip_ws_at_eof); \
  RUN(test_charbuf_predicates); \
  RUN(test_charbuf_pred_get); \
  RUN(test_charbuf_slow_source); \
  RUN(test_charbuf_error); \
  RUN(test_charbuf_free_twice)

TESTS(charbuf) { RUN_CHARBUF_TESTS(); }
