#include "../buffer.h"
#include <stdlib.h>

#ifdef HAVE_LIBBZ2
#include <bzlib.h>

#define BZ_BLOCK_SIZE 1024

typedef struct {
  bz_stream strm;
  int a;
  buffer* b;
  char buf[1024];
} bz_ctx;

static ssize_t
buffer_bzread_op(fd_t fd, void* data, size_t n, buffer* b) {
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

  if(ret != BZ_OK) return -1;

  return 0;
}

static ssize_t
buffer_bzwrite_op(fd_t fd, void* data, size_t n, buffer* b) {
  bz_ctx* ctx = b->cookie;
  bz_stream* strm = &ctx->strm;
  int ret;
  buffer* other = ctx->b;
  ssize_t r, a = other->a - other->p;
  int eof = 0;

  strm->next_in = data;
  strm->avail_in = n;
  strm->next_out = (char*)&other->x[other->p];
  strm->avail_out = a;

  ctx->a = BZ_RUN;

  ret = BZ2_bzCompress(strm, ctx->a);

  if(strm->avail_in == 0 || ret == BZ_STREAM_END) {
    r = n - strm->avail_in;

    if(r > 0) {
      a = (other->a - other->p) - strm->avail_out;
      other->p += a;
      return r;
    }

  } else if(ret != BZ_OK)
    return -1;

  return 0;
}

static void
buffer_bz_close(buffer* b) {
  bz_ctx* ctx = b->cookie;
  buffer* other = ctx->b;
  bz_stream* strm = &ctx->strm;
  int ret;
  ssize_t a;

  ctx->a = BZ_FLUSH;

  strm->next_in = (char*)&b->x[b->p];
  strm->avail_in = b->n - b->p;

  do {
    strm->next_out = (char*)&other->x[other->p];
    strm->avail_out = a = other->a - other->p;

    ret = BZ2_bzCompress(strm, ctx->a);

    if(ret == BZ_FLUSH_OK) ctx->a = BZ_FINISH;
    if(ret == BZ_FINISH_OK) break;

  } while(ret != BZ_STREAM_END);

  other->p += a - strm->avail_out;

  buffer_flush(other);

  b->deinit = 0;
  buffer_close(b);
}

int
buffer_bz2(buffer* b, buffer* other, int compress) {

  bz_ctx* ctx = calloc(1, sizeof(bz_ctx));
  if(ctx == NULL) return 0;

  ctx->b = other;

  buffer_init(b,
              (buffer_op_sys*)(compress ? (void*)&buffer_bzwrite_op : (void*)&buffer_bzread_op),
              -1,
              ctx->buf,
              sizeof(ctx->buf));
  b->cookie = ctx;
  b->deinit = &buffer_bz_close;

  int ret = compress ? BZ2_bzCompressInit(&ctx->strm, compress, 0, 1) : BZ2_bzDecompressInit(&ctx->strm, 0, 0);

  if(ret != BZ_OK) return 0;

  return 1;
}
#endif
