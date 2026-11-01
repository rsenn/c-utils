#include "../buffer.h"
#include "../alloc.h"
#include "../uint8.h"
#include <stdlib.h>
#include <errno.h>

#if HAVE_BROTLI
#ifdef __BORLANDC__
#define _MSC_VER 1500
#endif

#define BROTLI_SHARED_COMPILATION

#include <brotli/decode.h>
#include <brotli/encode.h>
#include <brotli/port.h>
#include <brotli/types.h>

typedef struct {
  void* state;
  buffer* b;
} brotli_ctx;

/* decompress: input comes from the other buffer, output goes to data */
static ssize_t
buffer_brotli_read(fd_type fd, void* data, size_t n, buffer* b) {
  brotli_ctx* ctx = b->cookie;
  buffer* other = ctx->b;

  for(;;) {
    BrotliDecoderResult ret;
    const uint8_t* next_in;
    uint8_t* next_out = data;
    size_t avail_in, avail_out = n;
    ssize_t r = 1;

    if(other->p >= other->n)
      r = buffer_feed(other);

    next_in = (const uint8_t*)&other->x[other->p];
    avail_in = other->p < other->n ? other->n - other->p : 0;

    ret = BrotliDecoderDecompressStream(ctx->state, &avail_in, &next_in, &avail_out, &next_out, 0);
    other->p = (size_t)((const char*)next_in - other->x);

    if(ret == BROTLI_DECODER_RESULT_ERROR)
      return -1;
    if(avail_out < n)
      return (ssize_t)(n - avail_out);
    if(ret == BROTLI_DECODER_RESULT_SUCCESS)
      return 0;
    if(r < 0)
      return -1;
    if(r == 0 && avail_in == 0)
      return 0; /* truncated input: end of data */
  }
}

/* compress: whatever is written is encoded and passed on to the other buffer */
static ssize_t
buffer_brotli_write(fd_type fd, void* data, size_t n, buffer* b) {
  brotli_ctx* ctx = b->cookie;
  const uint8_t* next_in = data;
  size_t avail_in = n;

  while(avail_in > 0) {
    uint8_t out[4096];
    uint8_t* next_out = out;
    size_t avail_out = sizeof(out);

    if(!BrotliEncoderCompressStream(ctx->state, BROTLI_OPERATION_PROCESS, &avail_in, &next_in, &avail_out, &next_out, 0))
      return -1;
    if(next_out > out)
      if(buffer_put(ctx->b, (const char*)out, (size_t)(next_out - out)) < 0)
        return -1;
  }
  return (ssize_t)n;
}

static void
buffer_brotli_close(buffer* b) {
  brotli_ctx* ctx = b->cookie;

  if(b->op == (buffer_op_proto*)(void*)&buffer_brotli_write) {
    size_t avail_in = 0;
    const uint8_t* next_in = NULL;

    do {
      uint8_t out[4096];
      uint8_t* next_out = out;
      size_t avail_out = sizeof(out);

      if(!BrotliEncoderCompressStream(ctx->state, BROTLI_OPERATION_FINISH, &avail_in, &next_in, &avail_out, &next_out, 0))
        break;
      if(next_out > out)
        buffer_put(ctx->b, (const char*)out, (size_t)(next_out - out));
    } while(!BrotliEncoderIsFinished(ctx->state));

    BrotliEncoderDestroyInstance(ctx->state);
    buffer_flush(ctx->b);
  } else {
    BrotliDecoderDestroyInstance(ctx->state);
  }

  alloc_free(b->x);
  alloc_free(ctx);
  b->x = NULL;
  b->deinit = 0;
}

/* compress != 0: writes to b are compressed (quality 0..11) into other;
 * compress == 0: reads from b decompress what is read from other. */
int
buffer_brotli(buffer* b, buffer* other, int compress) {
  brotli_ctx* ctx;
  char* buf;

  if((buf = alloc(4096)) == NULL)
    return 0;

  if((ctx = alloc_zero(sizeof(brotli_ctx))) == NULL) {
    alloc_free(buf);
    return 0;
  }

  if(compress) {
    ctx->state = BrotliEncoderCreateInstance(0, 0, ctx);
    if(ctx->state)
      BrotliEncoderSetParameter(ctx->state, BROTLI_PARAM_QUALITY, compress);
  } else {
    ctx->state = BrotliDecoderCreateInstance(0, 0, ctx);
  }

  if(ctx->state == NULL) {
    alloc_free(buf);
    alloc_free(ctx);
    return 0;
  }

  ctx->b = other;

  buffer_init(b, (buffer_op_proto*)(void*)(compress ? &buffer_brotli_write : &buffer_brotli_read), -1, buf, 4096);
  b->cookie = ctx;
  b->deinit = &buffer_brotli_close;

  return 1;
}
#endif
