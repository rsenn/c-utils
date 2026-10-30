#if HAVE_LIBBZ2
#include <bzlib.h>
#endif

#include "../buffer.h"
#include <stdlib.h>

#if HAVE_LIBBZ2

#define BZ_BLOCK_SIZE 1024

typedef struct {
  bz_stream strm;
  int a;
  buffer* b;
  char buf[1024];
} bz_ctx;

static ssize_t
buffer_bzread_op(fd_type fd, void* data, size_t n, buffer* b) {
  bz_ctx* ctx = b->cookie;
  bz_stream* strm = &ctx->strm;
  int ret;
  ssize_t r;
  size_t a;
  int eof = 0;

  if((r = buffer_prefetch(ctx->b, BZ_BLOCK_SIZE)) > 0) {
  } else {
    return r;
  }

  strm->next_in = (char*)&ctx->b->x[ctx->b->p];
  strm->avail_in = a = ctx->b->n - ctx->b->p;
  strm->next_out = data;
  strm->avail_out = n;

  ctx->a = !eof ? BZ_RUN : BZ_FINISH;

  do {
    ret = BZ2_bzDecompress(strm);
  } while(ctx->a == BZ_FINISH && ret != BZ_STREAM_END);

  if(/* strm->avail_out == 0 */
     ret == BZ_OK || ret == BZ_STREAM_END) {
    ctx->b->p += a - strm->avail_in;

    return n - strm->avail_out;
  }

  if(ret != BZ_OK)
    return -1;

  return 0;
}

/* compresses all n bytes, bzip2 takes no input while its output is full, so
 * the other buffer is flushed and the call repeated until the input is used */
static ssize_t
buffer_bzwrite_op(fd_type fd, void* data, size_t n, buffer* b) {
  bz_ctx* ctx = b->cookie;
  bz_stream* strm = &ctx->strm;
  buffer* other = ctx->b;
  size_t a;

  strm->next_in = data;
  strm->avail_in = n;

  while(strm->avail_in > 0) {
    if(!buffer_SPACE(other) && buffer_flush(other) < 0)
      return -1;

    a = buffer_SPACE(other);
    strm->next_out = (char*)buffer_PEEK(other);
    strm->avail_out = a;

    if(BZ2_bzCompress(strm, BZ_RUN) != BZ_RUN_OK)
      return -1;

    other->p += a - strm->avail_out;
  }

  return n;
}

/* close writes what the compressor still holds (BZ_FINISH) and the stream
 * trailer; without it bzip2 keeps the whole block and the output is empty */
static void
buffer_bz_close(buffer* b) {
  bz_ctx* ctx = b->cookie;
  buffer* other = ctx->b;
  bz_stream* strm = &ctx->strm;

  if(b->op == (void*)&buffer_bzwrite_op) {
    int ret;
    size_t a;

    buffer_flush(b);

    strm->next_in = 0;
    strm->avail_in = 0;

    do {
      if(!buffer_SPACE(other))
        buffer_flush(other);

      a = buffer_SPACE(other);
      strm->next_out = (char*)buffer_PEEK(other);
      strm->avail_out = a;

      ret = BZ2_bzCompress(strm, BZ_FINISH);
      other->p += a - strm->avail_out;
    } while(ret == BZ_FINISH_OK);

    BZ2_bzCompressEnd(strm);
    buffer_flush(other);
  } else {
    BZ2_bzDecompressEnd(strm);
  }

  b->deinit = 0;
  b->cookie = 0;
  free(ctx);
}

int
buffer_bz2(buffer* b, buffer* other, int compress) {
  int ret;
  bz_ctx* ctx = calloc(1, sizeof(bz_ctx));

  if(ctx == NULL)
    return 0;

  ctx->b = other;

  buffer_init(b, 0, -1, ctx->buf, sizeof(ctx->buf));
  b->cookie = ctx;
  b->deinit = &buffer_bz_close;
  b->op = (void*)(compress ? &buffer_bzwrite_op : &buffer_bzread_op);

  ret = compress ? BZ2_bzCompressInit(&ctx->strm, compress, 0, 1) : BZ2_bzDecompressInit(&ctx->strm, 0, 0);

  if(ret != BZ_OK)
    return 0;

  return 1;
}
#endif
