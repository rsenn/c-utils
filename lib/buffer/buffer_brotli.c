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
  size_t a, avail_in, avail_out;
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
  const uint8* next_in;
  uint8* next_out;
  ssize_t r;
  size_t a, pos, avail_in, avail_out;
  BROTLI_BOOL ret;

  r = 0;
  pos = other->p /* == other->n ? 0 : other->n*/;
  a = other->a - pos;
  next_in = data;
  avail_in = n;
  next_out = (uint8*)&other->x[pos];
  avail_out = a;

  do {
    ret = BrotliEncoderCompressStream(ctx->state,
                                      /*n < b->a ? BROTLI_OPERATION_FINISH :*/ BROTLI_OPERATION_PROCESS,
                                      &avail_in,
                                      &next_in,
                                      &avail_out,
                                      &next_out,
                                      0);
  } while(avail_in > 0 && avail_out > 0 && ret != BROTLI_FALSE);

  ret = BrotliEncoderHasMoreOutput(ctx->state);

  if(avail_out < a)
    other->p = pos + (a - avail_out);
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
  ssize_t r;
  int eof = 0;
  const uint8* next_in;
  uint8* next_out;
  size_t a, avail_in, avail_out;
  BROTLI_BOOL ret;

  r = 0;
  next_in = (const uint8*)&b->x[b->p];
  avail_in = b->n - b->p;
  a = other->a - other->p;
  next_out = (uint8*)&other->x[other->p];
  avail_out = a;

  if(b->op == (buffer_op_proto*)&buffer_brotli_write) {

    do {
      ret = BrotliEncoderCompressStream(ctx->state, BROTLI_OPERATION_FINISH, &avail_in, &next_in, &avail_out, &next_out, 0);
    } while(ret != BROTLI_FALSE && avail_in > 0);

    BrotliEncoderDestroyInstance(ctx->state);
  } else {
    do {

      ret = BrotliDecoderDecompressStream(ctx->state, &avail_in, &next_in, &avail_out, &next_out, 0);
    } while(ret != BROTLI_FALSE && avail_in > 0);
    BrotliDecoderDestroyInstance(ctx->state);
  }

  if(avail_out < a)
    other->p += a - avail_out;

  buffer_flush(other);

  b->deinit = 0;
  // buffer_close(b);
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

  buffer_init(b, (buffer_op_sys*)(void*)(compress ? &buffer_brotli_write : &buffer_brotli_read), -1, buf, 1024);
  b->cookie = ctx;
  b->deinit = &buffer_brotli_close;

  return 1;
}
#endif
