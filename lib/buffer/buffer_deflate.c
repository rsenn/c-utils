#include "../windoze.h"
#if WINDOWS
#define ZLIB_DLL 1
#endif
#include "../buffer.h"
#include "../byte.h"
#include "../str.h"
#include <stdlib.h>
#include <time.h>

#if HAVE_ZLIB
#include <zlib.h>

typedef struct {
  z_stream z;
  gz_header hdr;
  buffer* other;
  char buf[1024];
} deflate_ctx;

/* compresses all n bytes, the other buffer is flushed whenever it is full */
static ssize_t
buffer_deflate_write(fd_type fd, const void* data, size_t n, buffer* b) {
  deflate_ctx* ctx = b->cookie;
  z_stream* z = &ctx->z;
  buffer* other = ctx->other;
  size_t a;

  z->avail_in = n;
  z->next_in = (void*)data;

  while(z->avail_in > 0) {
    if(!buffer_SPACE(other) && buffer_flush(other) < 0)
      return -1;

    a = buffer_SPACE(other);
    z->next_out = (void*)buffer_PEEK(other);
    z->avail_out = a;

    if(deflate(z, Z_NO_FLUSH) == Z_STREAM_ERROR)
      return -1;

    other->p += a - z->avail_out;
  }

  return n;
}

/* close writes what deflate still holds and the gzip trailer (Z_FINISH), a
 * stream without it is rejected by gzip as truncated */
static void
buffer_deflate_close(buffer* b) {
  deflate_ctx* ctx = b->cookie;
  z_stream* z = &ctx->z;
  buffer* other = ctx->other;
  size_t a;
  int ret;

  buffer_flush(b);

  z->next_in = 0;
  z->avail_in = 0;

  do {
    if(!buffer_SPACE(other))
      buffer_flush(other);

    a = buffer_SPACE(other);
    z->next_out = (void*)buffer_PEEK(other);
    z->avail_out = a;

    ret = deflate(z, Z_FINISH);
    other->p += a - z->avail_out;
  } while(ret == Z_OK || ret == Z_BUF_ERROR);

  deflateEnd(z);
  buffer_flush(other);

  b->deinit = NULL;
  b->cookie = NULL;
  free(ctx);
}

int
buffer_deflate(buffer* b, buffer* out, int level) {
  int ret;
  z_stream* z;
  deflate_ctx* ctx = calloc(1, sizeof(deflate_ctx));

  if(ctx == NULL)
    return 0;

  ctx->other = out;

  z = &ctx->z;
  z->zalloc = Z_NULL;
  z->zfree = Z_NULL;
  z->opaque = Z_NULL;

  byte_zero(b, sizeof(buffer));

  buffer_init(b, (buffer_op_fn*)&buffer_deflate_write, -1, ctx->buf, sizeof(ctx->buf));
  b->cookie = ctx;
  b->deinit = &buffer_deflate_close;

  ret = deflateInit2(&ctx->z, level, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY);

  if(ret != Z_OK) {
    free(ctx);
    b->deinit = NULL;
    b->cookie = NULL;
    return 0;
  }

  byte_zero(&ctx->hdr, sizeof(gz_header));
  ctx->hdr.time = time(NULL);
  ctx->hdr.os = 3;

  deflateSetHeader(&ctx->z, &ctx->hdr);

  return 1;
}
#endif
