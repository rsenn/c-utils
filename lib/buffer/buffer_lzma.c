#include "../buffer.h"
#include <stdlib.h>

#ifdef HAVE_LIBLZMA
#include <lzma.h>

#define LZMA_BLOCK_SIZE 1024

typedef struct {
  lzma_stream strm;
  lzma_action a;
  buffer* b;
  char buf[1024];
} lzma_ctx;

static ssize_t
buffer_lzmaread_op(int fd, void* data, size_t n, buffer* b) {
  lzma_ctx* ctx = b->cookie;
  lzma_stream* strm = &ctx->strm;
  ssize_t r;
  size_t a;
  int eof = 0;

  if((r = buffer_prefetch(ctx->b, LZMA_BLOCK_SIZE)) > 0) {
    if(r < LZMA_BLOCK_SIZE) eof = 1;
  } else {
    return r;
  }

  a = ctx->b->n - ctx->b->p;

  strm->next_in = (uint8_t*)&ctx->b->x[ctx->b->p];
  strm->avail_in = a;
  strm->next_out = data;
  strm->avail_out = n;

  ctx->a = !eof ? LZMA_RUN : LZMA_FINISH;

  lzma_ret ret = lzma_code(strm, ctx->a);

  if(strm->avail_out == 0 || ret == LZMA_STREAM_END) {

    ctx->b->p += a - strm->avail_in;

    return n - strm->avail_out;
  }

  if(ret != LZMA_OK) return -1;

  return 0;
}

static ssize_t
buffer_lzmawrite_op(int fd, const void* data, size_t n, buffer* b) {
  lzma_ctx* ctx = b->cookie;
  lzma_stream* strm = &ctx->strm;
  buffer* other = ctx->b;
  ssize_t r, a;
  int eof = 0;

  strm->next_in = data;
  strm->avail_in = n;

  a = other->a - other->n;

  strm->next_out = (uint8_t*)&other->x[other->n];
  strm->avail_out = a;

  ctx->a = LZMA_RUN;

  lzma_ret ret = lzma_code(strm, ctx->a);

  if(strm->avail_in == 0 || ret == LZMA_STREAM_END) {
    r = n - strm->avail_in;

    if(r > 0) {
      a = (other->a - other->p) - strm->avail_out;
      other->p += a;
      return r;
    }

  } else if(ret != LZMA_OK)
    return -1;

  return 0;
}

static void
buffer_lzma_close(buffer* b) {
  lzma_ctx* ctx = b->cookie;
  lzma_stream* strm = &ctx->strm;

  ctx->a = LZMA_FINISH;

  lzma_ret ret = lzma_code(strm, ctx->a);
}

int
buffer_lzma(buffer* b, buffer* other, int compress) {

  lzma_options_lzma opt_lzma2;
  if(lzma_lzma_preset(&opt_lzma2, LZMA_PRESET_DEFAULT)) { return 0; }

  lzma_filter filters[] = {
      {.id = LZMA_FILTER_X86, .options = NULL},
      {.id = LZMA_FILTER_LZMA2, .options = &opt_lzma2},
      {.id = LZMA_VLI_UNKNOWN, .options = NULL},
  };
  lzma_ctx* ctx = calloc(1, sizeof(lzma_ctx));
  if(ctx == NULL) return 0;

  ctx->b = other;

  buffer_init(b,
              compress ? &buffer_lzmawrite_op : &buffer_lzmaread_op,
              -1,
              ctx->buf,
              sizeof(ctx->buf));
  b->cookie = ctx;
  b->deinit = &buffer_lzma_close;

  lzma_ret ret =
      compress ? lzma_stream_encoder(&ctx->strm, filters, LZMA_CHECK_CRC64)
               : lzma_stream_decoder(&ctx->strm, UINT64_MAX, LZMA_CONCATENATED);

  if(ret != LZMA_OK) return 0;

  return 1;
}
#endif
