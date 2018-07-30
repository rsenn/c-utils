#include "../buffer.h"
#include <stdlib.h>

#ifdef HAVE_ZLIB
#include <zlib.h>

typedef struct {
  z_stream z;
  gz_header hdr;
  buffer* other;
  char buf[1024];
} deflate_ctx;

static ssize_t
buffer_deflate_write(int fd, const void* data, size_t n, buffer* b) {
  deflate_ctx* ctx = b->cookie;
  z_stream* z = &ctx->z;

  z->avail_in = n;
  z->next_in = (void*)data;

again:
  z->next_out = (void*)&ctx->other->x[ctx->other->p];
  z->avail_out = ctx->other->a - ctx->other->p;

  int ret = deflate(&ctx->z, 0);

  if(ret == Z_OK && z->avail_out == 0) {
    buffer_flush(ctx->other);
    goto again;
  }

  if(ret == Z_STREAM_ERROR) return -1;

  ssize_t have = n - z->avail_in;

  ctx->other->p += (ctx->other->a - ctx->other->p) - z->avail_out;

  return have;
}

static void
buffer_deflate_close(buffer* b) {
  deflate_ctx* ctx = b->cookie;
 
  int ret, wr;
  
  do { 
    ctx->z.next_out = &ctx->other->x[ctx->other->p];
    ctx->z.avail_out = ctx->other->a - ctx->other->p;

    ret = deflate(&ctx->z, Z_FINISH);   

   wr = (ctx->other->a - ctx->other->p) - ctx->z.avail_out;
   ctx->other->p += wr;

  } while(ret == Z_OK);

  buffer_flush(ctx->other);

  b->deinit = NULL;
  buffer_close(b);
}

int
buffer_deflate(buffer* b, buffer* out, int level) {

  deflate_ctx* ctx = calloc(1, sizeof(deflate_ctx));
  if(ctx == NULL) return 0;

  ctx->other = out;

  z_stream* z = &ctx->z;
  z->zalloc = Z_NULL;
  z->zfree = Z_NULL;
  z->opaque = Z_NULL;

  byte_zero(b, sizeof(buffer));

  buffer_init(b, &buffer_deflate_write, -1, ctx->buf, sizeof(ctx->buf));
  b->cookie = ctx;
  b->deinit = &buffer_deflate_close;

  int ret = deflateInit2(&ctx->z, level, Z_DEFLATED, 15+16, 8, Z_DEFAULT_STRATEGY);
  //int ret = deflateInit2(&ctx->z, level, Z_DEFLATED, 8+16, 8, Z_DEFAULT_STRATEGY);
  //
  byte_zero(&ctx->hdr, sizeof(gz_header));
  ctx->hdr.text = 0;
  ctx->hdr.time = time(NULL);
  ctx->hdr.os = 1;
  ctx->hdr.name = "test.c";
  ctx->hdr.name_max = str_len(ctx->hdr.name) + 1;

  deflateSetHeader(&ctx->z, &ctx->hdr);

  if(ret != Z_OK) return 0;

  return 1;
}
#endif
