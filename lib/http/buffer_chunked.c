#include "../buffer.h"
#include "../byte.h"
#include "../util.h"
#include "../uint64.h"
#include "../scan.h"
#include <stdlib.h>

typedef struct {
  size_t chunk_length;
  size_t data_pos;
  buffer* other;
  char buf[1024];
} chunked;

static void
buffer_chunked_close(buffer* b) {
  chunked* ctx = b->cookie;
  buffer* other = ctx->other;

  b->deinit = 0;
  buffer_close(b);
}

static ssize_t
buffer_chunked_op(fd_type fd, void* data, size_t n, buffer* b) {
  chunked* ctx = b->cookie;
  ssize_t r;

  if(ctx->chunk_length == 0) {
    char numbuf[1024];
    if((r = buffer_getline(ctx->other, numbuf, sizeof(numbuf))) < 0)
      return r;

    numbuf[r] = '\0';

    scan_xlonglong(numbuf, &ctx->chunk_length);
    ctx->data_pos = 0;
  } else {
    if((r = buffer_feed(ctx->other)) < 0)
      return r;

    size_t remain = ctx->chunk_length - ctx->data_pos;
    size_t len = buffer_LEN(ctx->other);
    char* x = buffer_PEEK(ctx->other);
    r = MIN(len, n);
    r = MIN(r, remain);

    if(r > 0) {
      byte_copy(data, len, x);
      ctx->data_pos += r;
    }
  }
  return r;
}

int
buffer_chunked(buffer* b, buffer* other) {
  int ret;
  chunked* ctx = calloc(1, sizeof(chunked));

  if(ctx == NULL)
    return 0;

  ctx->other = other;

  buffer_init(b, 0, -1, ctx->buf, sizeof(ctx->buf));
  b->cookie = ctx;
  b->deinit = &buffer_chunked_close;
  b->op = buffer_chunked_op;

  return 1;
}
