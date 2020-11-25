#include "../buffer.h"
#include "../alloc.h"
#include "../uint8.h"
#include <stdlib.h>
#include <errno.h>

#ifdef HAVE_BROTLI
#include <brotli/decode.h>
#include <brotli/encode.h>
#include <brotli/port.h>
#include <brotli/types.h>

typedef struct {
  void* state;
  buffer* b;
} brotli_ctx;

static ssize_t
buffer_brotli_read(fd_t fd, void* data, size_t n, buffer* b) {
  brotli_ctx* ctx = b->cookie;
  BrotliDecoderResult ret;
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

  while(avail_out > 0) {
    ret = BrotliDecoderDecompressStream(ctx->state, &avail_in, &next_in, &avail_out, &next_out, 0);

    if(ret == BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT) {
      errno = EAGAIN;
      return -1;
    }
    if(ret == BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT) {
      size_t offset = (char*)data - b->x;
      if(alloc_re((void**)&b->x, b->a, b->a << 1)) {
        b->a <<= 1;
        next_out = data = (char*)b->x + offset;
        avail_out = n = b->a - offset;
        continue;
      }
    }
    if(avail_in == 0 || ret == BROTLI_DECODER_RESULT_SUCCESS || ret == BROTLI_DECODER_RESULT_ERROR)
      break;
  }

  if(avail_in < a)
    ctx->b->p += a - avail_in;

  if(ret == BROTLI_DECODER_RESULT_ERROR)
    return -1;

  if(avail_out < n)
    return n - avail_out;

  return 0;
}

static ssize_t
buffer_brotli_write(fd_t fd, void* data, size_t n, buffer* b) {
  brotli_ctx* ctx = b->cookie;
  buffer* other = ctx->b;
  ssize_t r = 0;
  const uint8* next_in = data;
  size_t pos = other->p == other->n ? 0 : other->n;
  ssize_t a = other->a - pos;
  uint8* next_out = (uint8*)&other->x[pos];
  size_t avail_in = n;
  size_t avail_out = a;
  BROTLI_BOOL ret;

  do {

    ret = BrotliEncoderCompressStream(ctx->state, n < b->a ? BROTLI_OPERATION_FINISH : BROTLI_OPERATION_PROCESS, &avail_in, &next_in, &avail_out, &next_out, 0);
  } while(avail_in > 0 && avail_out > 0 && ret != BROTLI_FALSE);

  ret = BrotliEncoderHasMoreOutput(ctx->state);

  if(avail_out < a)
    other->n = pos + (a - avail_out);

  if(avail_in < n)
    r = n - avail_in;

  if(ret == BROTLI_FALSE)
    return -1;

  return r;
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

  if(b->op == (buffer_op_proto*)&buffer_brotli_write) {

    do {
      next_out = (uint8*)&other->x[other->p];
      avail_out = a = other->a - other->p;

      ret = BrotliEncoderCompressStream(ctx->state, BROTLI_OPERATION_FLUSH, &avail_in, &next_in, &avail_out, &next_out, 0);
    } while(ret != BROTLI_FALSE && avail_in > 0);

    BrotliEncoderDestroyInstance(ctx->state);
  } else {
    do {
      next_out = (uint8*)&other->x[other->p];
      avail_out = a = other->a - other->p;

      ret = BrotliDecoderDecompressStream(ctx->state, &avail_in, &next_in, &avail_out, &next_out, 0);
    } while(ret != BROTLI_FALSE && avail_in > 0);
    BrotliDecoderDestroyInstance(ctx->state);
  }

  if(avail_out < a)
    other->p += a - avail_out;

  buffer_flush(other);

  b->deinit = 0;
  buffer_close(b);
}

int
buffer_brotli(buffer* b, buffer* other, int compress) {
  brotli_ctx* ctx;
  char* buf;

  if((buf = alloc(1024)) == NULL)
    return 0;

  ctx = alloc_zero(sizeof(brotli_ctx));
  if(ctx == NULL)
    return 0;

  if(compress) {
    ctx->state = BrotliEncoderCreateInstance(0, 0, ctx);
    BrotliEncoderSetParameter(ctx->state, BROTLI_PARAM_QUALITY, compress);
  } else {
    ctx->state = BrotliDecoderCreateInstance(0, 0, ctx);
  }

  ctx->b = other;

  buffer_init(b, (buffer_op_proto*)(compress ? &buffer_brotli_write : &buffer_brotli_read), -1, buf, 1024);
  b->cookie = ctx;
  b->deinit = &buffer_brotli_close;

  return 1;
}
#endif
