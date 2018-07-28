#include <stdlib.h>
#include "../buffer.h"

#ifdef HAVE_ZLIB
#include <zlib.h>

typedef struct {
  z_stream z;
  buffer* in;
  char buf[1024];
} deflate_ctx;

ssize_t
buffer_zlib(int fd, void* data, size_t n, buffer* b) {
  deflate_ctx* ctx = b->cookie;
  z_stream* z = &ctx->z;
  char tmpbuf[1024];

  z->avail_in = buffer_get(ctx->in, tmpbuf, sizeof(tmpbuf));
  z->next_in = (void*)tmpbuf;
  z->next_out = data;
  z->avail_out = n;

  int ret = deflate(&ctx->z, 0);

  if(ret == Z_STREAM_ERROR) return -1;

  ssize_t have = n - z->avail_out;

  return have;
}

int
buffer_deflate(buffer* b, buffer* in) {

  deflate_ctx* ctx = calloc(1, sizeof(deflate_ctx));
  if(ctx == NULL) return 0;

  ctx->in = in;

  z_stream* z = &ctx->z;
  z->zalloc = Z_NULL;
  z->zfree = Z_NULL;
  z->opaque = Z_NULL;

  b->op = (void*)&buffer_zlib;
  b->p = b->n = 0;
  b->cookie = ctx;

  b->x = ctx->buf;
  b->a = sizeof(ctx->buf);
  
  int ret = deflateInit(&ctx->z, 3);

  if(ret != Z_OK)
    return 0;

  return 1;
}
#endif
