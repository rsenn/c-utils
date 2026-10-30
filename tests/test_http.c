#include "unit_test.h"
#include "../lib/http.h"
#include "../lib/buffer.h"
#include "../lib/byte.h"

/* the bytes the encoder writes end up here */
static char sink[256];
static size_t sink_len;

static ssize_t
sink_write(fd_type fd, void* buf, size_t len, buffer* b) {
  (void)fd;
  (void)b;
  byte_copy(sink + sink_len, len, buf);
  sink_len += len;
  return (ssize_t)len;
}

/*
 * int buffer_chunked(buffer* b, buffer* other);
 */
TEST(test_buffer_chunked) {
  static const char text[] = "5\r\nhello\r\n6;ext=1\r\n world\r\nA\r\n0123456789\r\n0\r\nTrailer: x\r\n\r\nrest";
  buffer in, dec;
  char out[64], rest[8];
  size_t got = 0;
  ssize_t n;

  buffer_frombuf(&in, text, sizeof(text) - 1);
  ASSERT_NE(0, buffer_chunked(&dec, &in));

  while((n = buffer_get(&dec, out + got, sizeof(out) - got)) > 0)
    got += (size_t)n;

  ASSERT_EQ(0, n);
  ASSERT_EQ(21, got);
  ASSERT_EQ(0, byte_diff(out, got, "hello world0123456789"));

  /* what follows the last chunk is left in the other buffer */
  ASSERT_EQ(4, buffer_get(&in, rest, sizeof(rest)));
  ASSERT_EQ(0, byte_diff(rest, 4, "rest"));

  buffer_close(&dec);
}

/*
 * a cut off input is an error, not an end of file
 */
TEST(test_buffer_chunked_truncated) {
  static const char text[] = "5\r\nhel";
  buffer in, dec;
  char out[16];
  ssize_t n, total = 0;

  buffer_frombuf(&in, text, sizeof(text) - 1);
  buffer_chunked(&dec, &in);

  while((n = buffer_get(&dec, out, sizeof(out))) > 0)
    total += n;

  ASSERT_LT(n, 0);
  buffer_close(&dec);
}

/*
 * int buffer_chunked_encode(buffer* b, buffer* other);
 */
TEST(test_buffer_chunked_encode) {
  char x[128];
  buffer out, enc;

  sink_len = 0;
  buffer_init(&out, sink_write, -1, x, sizeof(x));
  ASSERT_NE(0, buffer_chunked_encode(&enc, &out));

  buffer_puts(&enc, "abc");
  buffer_flush(&enc);
  buffer_puts(&enc, "defgh");
  buffer_close(&enc);

  ASSERT_EQ(sizeof("3\r\nabc\r\n5\r\ndefgh\r\n0\r\n\r\n") - 1, sink_len);
  ASSERT_EQ(0, byte_diff(sink, sink_len, "3\r\nabc\r\n5\r\ndefgh\r\n0\r\n\r\n"));
}

#define RUN_HTTP_TESTS() \
  RUN(test_buffer_chunked); \
  RUN(test_buffer_chunked_truncated); \
  RUN(test_buffer_chunked_encode)

TESTS(http) { RUN_HTTP_TESTS(); }
