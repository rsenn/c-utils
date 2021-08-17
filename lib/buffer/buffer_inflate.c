#include "../windoze.h"
#if WINDOWS
#define ZLIB_DLL 1
#endif

#include "../buffer.h"
#include <stdlib.h>

#ifdef HAVE_ZLIB
#include <zlib.h>

typedef struct {
  z_stream z;
  buffer* other;
  char buf[1024];
} inflate_ctx;

static ssize_t
buffer_inflate_read(fd_t fd, void* data, size_t n, buffer* b) {
  int ret;
  ssize_t have;
  inflate_ctx* ctx = b->cookie;
  z_stream* z = &ctx->z;

  z->avail_in = ctx->other->n - ctx->other->p;
  z->next_in = (void*)&ctx->other->x[ctx->other->p];
  z->next_out = data;
  z->avail_out = n;

  ret = inflate(&ctx->z, 0);

  if(ret == Z_STREAM_ERROR)
    return -1;

  have = n - z->avail_out;

  return have;
}

int
buffer_inflate(buffer* b, buffer* in) {
  int ret;
  z_stream* z;

  inflate_ctx* ctx = calloc(1, sizeof(inflate_ctx));
  if(ctx == NULL)
    return 0;

  ctx->other = in;

  z = &ctx->z;
  z->zalloc = Z_NULL;
  z->zfree = Z_NULL;
  z->opaque = Z_NULL;

  b->op = (void*)&buffer_inflate_read;
  b->p = b->n = 0;
  b->cookie = ctx;

  b->x = ctx->buf;
  b->a = sizeof(ctx->buf);

  ret = inflateInit2(&ctx->z, 15 + 32);

  if(ret != Z_OK)
    return 0;

  return 1;
}
#endif
