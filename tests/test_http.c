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

/*
 * int http_header(http*, const char*, const char*);
 * int http_body(http*, const char*, size_t);
 * int http_chunk(http*, const char*, size_t);
 * int http_chunk_end(http*);
 */
TEST(test_http_request_body) {
  http h;
  http_request* req;

  http_init(&h, "example.com", 80);
  req = http_request_new(&h);
  req->type = POST;

  ASSERT_NE(0, http_header(&h, "Authorization", "Bearer x"));
  ASSERT_EQ(sizeof("Authorization: Bearer x\r\n") - 1, req->headers.len);
  ASSERT_EQ(0, byte_diff(req->headers.s, req->headers.len, "Authorization: Bearer x\r\n"));

  /* a body of known length, which excludes chunks */
  ASSERT_NE(0, http_body(&h, "abc", 3));
  ASSERT_NE(0, http_body(&h, "de", 2));
  ASSERT_EQ(5, req->body.len);
  ASSERT_EQ(0, http_chunked(&h));
  ASSERT_EQ(0, http_chunk(&h, "x", 1));

  /* chunks, which exclude a body */
  req = http_request_new(&h);
  req->type = POST;
  ASSERT_NE(0, http_chunked(&h));
  ASSERT_EQ(0, http_body(&h, "abc", 3));
  ASSERT_NE(0, http_chunk(&h, "hello", 5));
  ASSERT_NE(0, http_chunk(&h, "", 0));
  ASSERT_NE(0, http_chunk(&h, "0123456789abcdef", 16));
  ASSERT_NE(0, http_chunk_end(&h));
  ASSERT_EQ(0, http_chunk(&h, "late", 4));

  ASSERT_EQ(sizeof("5\r\nhello\r\n10\r\n0123456789abcdef\r\n0\r\n\r\n") - 1, req->body.len);
  ASSERT_EQ(0, byte_diff(req->body.s, req->body.len, "5\r\nhello\r\n10\r\n0123456789abcdef\r\n0\r\n\r\n"));
}

/*
 * a header with a line end in it would add headers of its own
 */
TEST(test_http_header_injection) {
  http h;
  http_request* req;

  http_init(&h, "example.com", 80);
  req = http_request_new(&h);

  ASSERT_EQ(0, http_header(&h, "X-A", "b\r\nX-Evil: 1"));
  ASSERT_EQ(0, http_header(&h, "X-A\nX-Evil", "1"));
  ASSERT_EQ(0, http_header(&h, "Bad:Name", "1"));
  ASSERT_EQ(0, http_header(&h, "", "1"));
  ASSERT_EQ(0, req->headers.len);

  ASSERT_NE(0, http_header(&h, "X-A", "fine"));
  ASSERT_EQ(sizeof("X-A: fine\r\n") - 1, req->headers.len);

  http_free(&h);
  ASSERT_EQ(NULL, h.request);
}

/* a response that is made of what the input buffer holds */
static void
recv_setup(http* h, buffer* in, const char* text, size_t n, http_transfer_type transfer, uint64 length) {
  http_init(h, "example.com", 80);
  http_response_new(h);
  h->q.in = *in;
  buffer_frombuf(&h->q.in, text, n);
  h->response->status = HTTP_RECV_DATA;
  h->response->transfer = transfer;
  h->response->content_length = length;
}

/*
 * ssize_t http_recv(http*, char*, size_t);
 */
TEST(test_http_recv_chunked) {
  static const char text[] = "5\r\nhello\r\n6;ext=1\r\n world\r\n0\r\nTrailer: x\r\n\r\n";
  buffer in;
  http h;
  char out[64];
  size_t got = 0;
  ssize_t n;

  recv_setup(&h, &in, text, sizeof(text) - 1, HTTP_TRANSFER_CHUNKED, 0);

  /* a small buffer makes it come in pieces */
  while((n = http_recv(&h, out + got, 4)) > 0)
    got += (size_t)n;

  ASSERT_EQ(0, n);
  ASSERT_EQ(11, got);
  ASSERT_EQ(0, byte_diff(out, got, "hello world"));
  ASSERT_EQ(HTTP_STATUS_FINISH, h.response->status);
}

TEST(test_http_recv_length) {
  static const char text[] = "0123456789EXTRA";
  buffer in;
  http h;
  char out[64];

  recv_setup(&h, &in, text, sizeof(text) - 1, HTTP_TRANSFER_LENGTH, 10);

  ASSERT_EQ(10, http_recv(&h, out, sizeof(out)));
  ASSERT_EQ(0, byte_diff(out, 10, "0123456789"));
  ASSERT_EQ(HTTP_STATUS_FINISH, h.response->status);
}

/* a chunked body that stops in the middle is an error */
TEST(test_http_recv_truncated) {
  static const char text[] = "5\r\nhel";
  buffer in;
  http h;
  char out[64];

  recv_setup(&h, &in, text, sizeof(text) - 1, HTTP_TRANSFER_CHUNKED, 0);

  ASSERT_EQ(3, http_recv(&h, out, sizeof(out)));
  ASSERT_EQ(0, http_recv(&h, out, sizeof(out)));
  ASSERT_NE(HTTP_STATUS_FINISH, h.response->status);

  h.response->status = HTTP_STATUS_CLOSED;
  ASSERT_LT(http_recv(&h, out, sizeof(out)), 0);
}

/* a body without a length ends with the connection */
TEST(test_http_recv_until_close) {
  static const char text[] = "all of it";
  buffer in;
  http h;
  char out[64];

  recv_setup(&h, &in, text, sizeof(text) - 1, HTTP_TRANSFER_UNDEF, (uint64)-1);

  ASSERT_EQ(9, http_recv(&h, out, sizeof(out)));
  ASSERT_EQ(0, http_recv(&h, out, sizeof(out)));
  ASSERT_NE(HTTP_STATUS_FINISH, h.response->status);

  h.response->status = HTTP_STATUS_CLOSED;
  ASSERT_EQ(0, http_recv(&h, out, sizeof(out)));
  ASSERT_EQ(HTTP_STATUS_FINISH, h.response->status);
}

#define RUN_HTTP_TESTS() \
  RUN(test_buffer_chunked); \
  RUN(test_buffer_chunked_truncated); \
  RUN(test_buffer_chunked_encode); \
  RUN(test_http_request_body); \
  RUN(test_http_header_injection); \
  RUN(test_http_recv_chunked); \
  RUN(test_http_recv_length); \
  RUN(test_http_recv_truncated); \
  RUN(test_http_recv_until_close)

TESTS(http) { RUN_HTTP_TESTS(); }
