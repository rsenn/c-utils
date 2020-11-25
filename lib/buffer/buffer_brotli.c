#include "../buffer.h"
#include "../alloc.h"
#include "../uint8.h"
#include <stdlib.h>

#ifdef HAVE_BROTLI
#include <brotli/decode.h>
#include <brotli/encode.h>
#include <brotli/port.h>
#include <brotli/types.h>

typedef struct {
  void* state;
  buffer* b;
  char buf[1024];
} brotli_ctx;

static ssize_t
buffer_brotliread_op(fd_t fd, void* data, size_t n, buffer* b) {
  brotli_ctx* ctx = b->cookie;
  BROTLI_BOOL ret;
  ssize_t r;
  size_t a;
  int eof = 0;
  size_t avail_in, avail_out;
  const uint8* next_in;
  uint8* next_out;

  if((r = buffer_prefetch(ctx->b, 1024)) > 0) {
  } else {
    return r;
  }

  next_in = (uint8*)&ctx->b->x[ctx->b->p];
  avail_in = a = ctx->b->n - ctx->b->p;
  next_out = data;
  avail_out = n;

  do {
    ret = BrotliDecoderDecompressStream(ctx->state, &avail_in, &next_in, &avail_out, &next_out, 0);
  } while(avail_in > 0 && ret != BROTLI_FALSE);

  if(ret == BROTLI_TRUE) {

    ctx->b->p += a - avail_in;

    return n - avail_out;
  }

  if(ret == BROTLI_FALSE)
    return -1;

  return 0;
}

static ssize_t
buffer_brotliwrite_op(fd_t fd, void* data, size_t n, buffer* b) {
  brotli_ctx* ctx = b->cookie;
  buffer* other = ctx->b;
  ssize_t r, a = other->a - other->p;
  int eof = 0;
  const uint8* next_in = data;
  uint8* next_out = (uint8*)&other->x[other->p];
  size_t avail_in = n;
  size_t avail_out = a;
  BROTLI_BOOL ret;

  ret =
      BrotliEncoderCompressStream(ctx->state, BROTLI_OPERATION_PROCESS, &avail_in, &next_in, &avail_out, &next_out, 0);

  if(avail_in == 0) {
    r = n - avail_in;

    if(r > 0) {
      a = (other->a - other->p) - avail_out;
      other->p += a;
      return r;
    }
  }

  if(ret == BROTLI_FALSE)
    return -1;

  return 0;
}

static void
buffer_brotli_close(buffer* b) {
  brotli_ctx* ctx = b->cookie;
  buffer* other = ctx->b;
  ssize_t r, a = other->a - other->p;
  int eof = 0;
  const uint8* next_in = &b->x[b->p];
  uint8* next_out;
  size_t avail_in = b->n - b->p;
  size_t avail_out;
  BROTLI_BOOL ret;

  next_in = (uint8*)&b->x[b->p];
  avail_in = b->n - b->p;

  if(b->op == (buffer_op_proto*)&buffer_brotliwrite_op) {

    do {
      next_out = (uint8*)&other->x[other->p];
      avail_out = a = other->a - other->p;

      ret = BrotliEncoderCompressStream(
          ctx->state, BROTLI_OPERATION_FINISH, &avail_in, &next_in, &avail_out, &next_out, 0);
    } while(ret != BROTLI_FALSE && avail_in > 0);
  } else {
    do {
      next_out = (uint8*)&other->x[other->p];
      avail_out = a = other->a - other->p;

      ret = BrotliDecoderDecompressStream(ctx->state, &avail_in, &next_in, &avail_out, &next_out, 0);
    } while(ret != BROTLI_FALSE && avail_in > 0);
  }

  other->p += a - avail_out;

  buffer_flush(other);

  b->deinit = 0;
  buffer_close(b);
}

int
buffer_brotli(buffer* b, buffer* other, int compress) {
  brotli_ctx* ctx;

  ctx = alloc_zero(sizeof(brotli_ctx));
  if(ctx == NULL)
    return 0;

  ctx->state = compress ? BrotliEncoderCreateInstance(0, 0, ctx) : BrotliDecoderCreateInstance(0, 0, ctx);

  ctx->b = other;

  buffer_init(b, 0, -1, ctx->buf, sizeof(ctx->buf));
  b->cookie = ctx;
  b->deinit = &buffer_brotli_close;

  b->op = (buffer_op_proto*)(compress ? &buffer_brotliwrite_op : &buffer_brotliread_op);

  return 1;
}
#endif
