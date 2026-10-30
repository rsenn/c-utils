#include "../http.h"
#include "../alloc.h"
#include "../http_internal.h"

static ssize_t
buffer_chunked_encode_op(fd_type fd, void* data, size_t n, buffer* b) {
  chunked* ctx = b->cookie;
  buffer* out = ctx->other;

  (void)fd;

  if(buffer_putxlong(out, n) < 0 || buffer_put(out, "\r\n", 2) < 0 || buffer_put(out, data, n) < 0 ||
     buffer_put(out, "\r\n", 2) < 0)
    return -1;

  return (ssize_t)n;
}

static void
buffer_chunked_encode_close(buffer* b) {
  chunked* ctx = b->cookie;

  b->deinit = 0;

  buffer_flush(b);
  buffer_put(ctx->other, "0\r\n\r\n", 5);
  buffer_flush(ctx->other);

  b->cookie = 0;
  alloc_free(ctx);
}

/* buffer_chunked_encode makes b write what is put into it to other as
 * chunks, one chunk for each flush. Closing b flushes it, writes the last
 * chunk and leaves other open. */
int
buffer_chunked_encode(buffer* b, buffer* other) {
  chunked* ctx = alloc_zero(sizeof(chunked));

  if(ctx == NULL)
    return 0;

  ctx->other = other;

  buffer_init(b, buffer_chunked_encode_op, -1, ctx->buf, sizeof(ctx->buf));
  b->cookie = ctx;
  b->deinit = &buffer_chunked_encode_close;

  return 1;
}
