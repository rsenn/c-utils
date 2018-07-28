#include <stdlib.h>
#include "../buffer.h"

#ifdef HAVE_LIBLZMA
#include <lzma.h>

typedef struct {
  lzma_stream z;
  lzma_action a;
  buffer* in;
  char buf[1024];
} lzma_ctx;

ssize_t
buffer_zlib(int fd, void* data, size_t n, buffer* b) {
  lzma_ctx* ctx = b->cookie;
  lzma_stream* strm = &ctx->z;
  char tmpbuf[1024];

  strm->next_in = tmpbuf;
  strm->avail_in = buffer_get(ctx->in, tmpbuf, sizeof(tmpbuf));
  strm->next_out = data;
  strm->avail_out = n;

  ctx->a = LZMA_RUN;

  lzma_ret ret = lzma_code(strm, ctx->a);

      if(strm->avail_out == 0 || ret == LZMA_STREAM_END)
            return sizeof(tmpbuf) - strm->avail_out;

  if(ret != LZMA_OK)
  return -1;

  return 0;
}

int
buffer_deflate(buffer* b, buffer* in) {

  lzma_ctx* ctx = calloc(1, sizeof(lzma_ctx));
  if(ctx == NULL) return 0;

  ctx->in = in;

  lzma_stream* z = &ctx->z;

  b->op = &buffer_zlib;
  b->p = b->n = 0;
  b->cookie = ctx;

  b->x = ctx->buf;
  b->a = sizeof(ctx->buf);

  int ret = deflateInit(&ctx->z, 3);

  if(ret != LZMA_OK)
    return 0;

  return 1;
}
#endif
