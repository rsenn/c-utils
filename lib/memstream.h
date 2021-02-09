#ifndef MEMSTREAM_H
#define MEMSTREAM_H

#include "buffer.h"
#include "alloc.h"
#include "byte.h"

typedef struct memstream_s {
  char** pdata;
  size_t* psize;
} memstream_ctx;

static inline void
memstream_free(buffer* b) {
  alloc_free(b->x);
  b->x = 0;
  alloc_free(b);
}

static inline ssize_t
memstream_write(int fd, void* data, size_t len, buffer* b) {
  memstream_ctx* ms = (memstream_ctx*)&b[1];
  size_t n = *ms->psize;
  if(alloc_re(ms->pdata, n, n + len)) {
    byte_copy((*ms->pdata) + n, len, data);
    return len;
  }
  return 0;
}

static inline buffer*
memstream_open(char** ptr, size_t* sizeloc) {
  buffer* b = alloc(sizeof(buffer) + sizeof(memstream_ctx));
  memstream_ctx* ms = (memstream_ctx*)&b[1];

  ms->pdata = ptr;
  ms->psize = sizeloc;

  b->cookie = ms;

  buffer_init_free(b, (buffer_op_fn*)&memstream_write, -1, alloc(1024), 1024);
  b->deinit = &memstream_free;
  return b;
}

#endif /* !defined(MEMSTREAM_H) */