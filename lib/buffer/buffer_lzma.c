#include "../buffer.h"
#include "../uint8.h"
#include <stdlib.h>

#ifdef HAVE_LIBLZMA
#define LZMA_API_STATIC 1
#include <lzma.h>

#ifdef __DMC__
#undef LZMA_VLI_UNKNOWN
#define LZMA_VLI_UNKNOWN 18446744073709551615ull
#endif

#define LZMA_BLOCK_SIZE 1024

typedef struct {
  lzma_stream strm;
  lzma_action a;
  buffer* b;
  char buf[1024];
} lzma_ctx;

static ssize_t
buffer_lzmaread_op(fd_t fd, void* data, size_t n, buffer* b) {
  lzma_ctx* ctx = b->cookie;
  lzma_stream* strm = &ctx->strm;
  lzma_ret ret;
  ssize_t r;
  size_t a;
  int eof = 0;

  if((r = buffer_prefetch(ctx->b, LZMA_BLOCK_SIZE)) > 0) {
  } else {
    return r;
  }

  strm->next_in = (uint8*)&ctx->b->x[ctx->b->p];
  strm->avail_in = a = ctx->b->n - ctx->b->p;
  strm->next_out = data;
  strm->avail_out = n;

  ctx->a = !eof ? LZMA_RUN : LZMA_FINISH;

  do {
    ret = lzma_code(strm, ctx->a);
  } while(ctx->a == LZMA_FINISH && ret != LZMA_STREAM_END);

  if(/* strm->avail_out == 0 */
     ret == LZMA_OK || ret == LZMA_STREAM_END) {

    ctx->b->p += a - strm->avail_in;

    return n - strm->avail_out;
  }

  if(ret != LZMA_OK) return -1;

  return 0;
}

static ssize_t
buffer_lzmawrite_op(fd_t fd, void* data, size_t n, buffer* b) {
  lzma_ctx* ctx = b->cookie;
  lzma_stream* strm = &ctx->strm;
  lzma_ret ret;
  buffer* other = ctx->b;
  ssize_t r, a = other->a - other->p;
  int eof = 0;

  strm->next_in = data;
  strm->avail_in = n;
  strm->next_out = (uint8*)&other->x[other->p];
  strm->avail_out = a;

  ctx->a = LZMA_RUN;

  ret = lzma_code(strm, ctx->a);

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
  buffer* other = ctx->b;
  lzma_stream* strm = &ctx->strm;
  lzma_ret ret;
  ssize_t a;

  ctx->a = LZMA_FINISH;

  strm->next_in = (uint8*)&b->x[b->p];
  strm->avail_in = b->n - b->p;

  do {
    strm->next_out = (uint8*)&other->x[other->p];
    strm->avail_out = a = other->a - other->p;

    ret = lzma_code(strm, ctx->a);
  } while(ret != LZMA_STREAM_END);

  other->p += a - strm->avail_out;

  buffer_flush(other);

  b->deinit = 0;
  buffer_close(b);
}

int
buffer_lzma(buffer* b, buffer* other, int compress) {
  lzma_ret ret;
  lzma_ctx* ctx;
  lzma_options_lzma opt_lzma2;
  lzma_filter f[3];
  
  f[0].id = 0x04; //LZMA_FILTER_X86;
  f[0].options = 0;
  
  f[1].id = 0x21; //LZMA_FILTER_LZMA2;
  f[1].options = &opt_lzma2;
  
  f[2].id = 0xffffffffffffffff; // LZMA_VLI_UNKNOWN;
  
  if(lzma_lzma_preset(&opt_lzma2, LZMA_PRESET_DEFAULT)) {
    return 0;
  }
  ctx = calloc(1, sizeof(lzma_ctx));
  if(ctx == NULL) return 0;

  ctx->b = other;

  buffer_init(b,
              (buffer_op_proto*)(compress ? (void*)&buffer_lzmawrite_op : (void*)&buffer_lzmaread_op),
              -1,
              ctx->buf,
              sizeof(ctx->buf));
  b->cookie = ctx;
  b->deinit = &buffer_lzma_close;

  ret = compress ? lzma_stream_encoder(&ctx->strm, f, LZMA_CHECK_CRC64)
                 : lzma_stream_decoder(&ctx->strm, 0xffffffffffffffff /*LZMA_VLI_UNKNOWN*/, LZMA_CONCATENATED);

  if(ret != LZMA_OK) return 0;

  return 1;
}
#endif

