#include "../http.h"
#include "../alloc.h"
#include "../byte.h"
#include "../scan.h"
#include "../http_internal.h"

#include <errno.h>

static void
buffer_chunked_close(buffer* b) {
  chunked* ctx = b->cookie;

  b->deinit = 0;
  b->cookie = 0;
  alloc_free(ctx);
}

static ssize_t
buffer_chunked_op(fd_type fd, void* data, size_t n, buffer* b) {
  chunked* ctx = b->cookie;
  buffer* in = ctx->other;
  char line[96];
  uint64 len;
  size_t r;

  (void)fd;

  for(;;) {
    if(ctx->state == CHUNKED_END)
      return 0;

    if(ctx->state == CHUNKED_SIZE) {
      /* chunk size in hex, an extension after a semicolon is ignored */
      if(chunked_line(in, line, sizeof(line)) < 0) {
        errno = EPIPE;
        return -1;
      }

      if(!scan_x64(line, &len)) {
        errno = EINVAL;
        return -1;
      }

      if(len == 0) {
        /* last chunk, the trailer lines end with an empty line */
        while(chunked_line(in, line, sizeof(line)) > 0) {
        }
        ctx->state = CHUNKED_END;
        return 0;
      }

      ctx->remaining = len;
      ctx->state = CHUNKED_DATA;
    }

    if(buffer_feed(in) <= 0) {
      errno = EPIPE;
      return -1;
    }

    r = buffer_LEN(in);
    if(r > n)
      r = n;
    if(r > ctx->remaining)
      r = (size_t)ctx->remaining;

    byte_copy(data, r, buffer_PEEK(in));
    buffer_SKIP(in, r);
    ctx->remaining -= r;

    /* the data of a chunk is followed by an end of line */
    if(ctx->remaining == 0) {
      if(chunked_line(in, line, sizeof(line)) < 0) {
        errno = EPIPE;
        return -1;
      }
      ctx->state = CHUNKED_SIZE;
    }
    return (ssize_t)r;
  }
}

/* buffer_chunked makes b read the data of the chunked transfer encoding in
 * other. At the last chunk b reads end of file, a cut off input is an error.
 * Closing b frees it and leaves other open. */
int
buffer_chunked(buffer* b, buffer* other) {
  chunked* ctx = alloc_zero(sizeof(chunked));

  if(ctx == NULL)
    return 0;

  ctx->other = other;

  buffer_init(b, buffer_chunked_op, -1, ctx->buf, sizeof(ctx->buf));
  b->cookie = ctx;
  b->deinit = &buffer_chunked_close;

  return 1;
}
