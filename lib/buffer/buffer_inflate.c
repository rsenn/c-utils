#include "../windoze.h"
#if WINDOWS
#define ZLIB_DLL 1
#endif

#include "../buffer.h"
#include <stdlib.h>

#if HAVE_ZLIB
#include <zlib.h>

typedef struct {
  z_stream z;
  buffer* other;
  int done;
  char buf[1024];
} inflate_ctx;

/* reads compressed data from the other buffer as it is needed, returns 0 at
 * the end of the stream and -1 for corrupt or truncated data */
static ssize_t
buffer_inflate_read(fd_type fd, void* data, size_t n, buffer* b) {
  inflate_ctx* ctx = b->cookie;
  z_stream* z = &ctx->z;
  buffer* other = ctx->other;
  ssize_t r;
  size_t used;
  int ret;

  while(!ctx->done) {
    if((r = buffer_feed(other)) < 0)
      return -1;

    if(r == 0)
      return -1;

    z->next_in = (void*)buffer_peek(other);
    z->avail_in = r;
    z->next_out = data;
    z->avail_out = n;

    ret = inflate(z, Z_NO_FLUSH);

    used = r - z->avail_in;
    buffer_seek(other, used);

    if(ret == Z_STREAM_END)
      ctx->done = 1;
    else if(ret != Z_OK && ret != Z_BUF_ERROR)
      return -1;

    if(z->avail_out < n)
      return n - z->avail_out;

    if(used == 0 && !ctx->done)
      return -1;
  }

  return 0;
}

static void
buffer_inflate_close(buffer* b) {
  inflate_ctx* ctx = b->cookie;

  inflateEnd(&ctx->z);
  b->deinit = NULL;
  b->cookie = NULL;
  free(ctx);
}

int
buffer_inflate(buffer* b, buffer* in) {
  inflate_ctx* ctx = calloc(1, sizeof(inflate_ctx));

  if(ctx == NULL)
    return 0;

  ctx->other = in;

  buffer_init(b, (buffer_op_fn*)&buffer_inflate_read, -1, ctx->buf, sizeof(ctx->buf));
  b->cookie = ctx;
  b->deinit = &buffer_inflate_close;

  if(inflateInit2(&ctx->z, 15 + 32) != Z_OK) {
    free(ctx);
    b->deinit = NULL;
    b->cookie = NULL;
    return 0;
  }

  return 1;
}
#endif
