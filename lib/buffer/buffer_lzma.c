#include <stdlib.h>
#include "../buffer.h"

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
    if(r < LZMA_BLOCK_SIZE)
      eof = 1;
  } else {
    return r;
  }
 
  a = ctx->b->n - ctx->b->p; 

  strm->next_in = &ctx->b->x[ctx->b->p];
  strm->avail_in = a; 
  strm->next_out = data;
  strm->avail_out = n;

  ctx->a = !eof ? LZMA_RUN : LZMA_FINISH;

  lzma_ret ret = lzma_code(strm, ctx->a);

      if(strm->avail_out == 0 || ret == LZMA_STREAM_END) {

           ctx->b->p += a - strm->avail_in;

            return n - strm->avail_out;
      }

  if(ret != LZMA_OK)
  return -1;

  return 0;
}

static ssize_t
buffer_lzmawrite_op(int fd, const void* data, size_t n, buffer* b) {
  lzma_ctx* ctx = b->cookie;
  lzma_stream* strm = &ctx->strm;
  buffer* other = ctx->b;
  ssize_t r;
  size_t a;
  int eof = 0;

  strm->next_in = data;
  strm->avail_in = n; 
  
  strm->next_out = &other->x[other->p];
  strm->avail_out = other->a - other->p;

  ctx->a = LZMA_RUN;

  lzma_ret ret = lzma_code(strm, ctx->a);

  if(strm->avail_out == 0 || ret == LZMA_STREAM_END) {

       other->p += (other->a - other->p) - strm->avail_out;

           return n - strm->avail_in;
      }

  if(ret != LZMA_OK)
  return -1;

return 0;
}

int
buffer_lzma(buffer* b, buffer* other, int compress) {

  lzma_ctx* ctx = calloc(1, sizeof(lzma_ctx));
  if(ctx == NULL) return 0;

  ctx->b = other;

  lzma_ret ret = 
    compress ?  lzma_stream_encoder(&ctx->strm, NULL, LZMA_CHECK_CRC64) : lzma_stream_decoder(&ctx->strm, UINT64_MAX, LZMA_CONCATENATED);

  if(ret != LZMA_OK)
    return 0;

  lzma_stream* strm = &ctx->strm;

  b->op = compress ? &buffer_lzmawrite_op : &buffer_lzmaread_op;
  b->p = b->n = 0;
  b->cookie = ctx;

  b->x = ctx->buf;
  b->a = sizeof(ctx->buf);

  return 1;
}
#endif
