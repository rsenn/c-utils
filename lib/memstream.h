#ifndef MEMSTREAM_H
#define MEMSTREAM_H

#include "buffer.h"
#include "alloc.h"
#include "byte.h"

typedef struct memstream_s {
  char** pdata;
  size_t* psize;
} memstream;

static inline void
memstream_free(buffer* b) {
  buffer_free(b);
  alloc_free(b);
}

static inline ssize_t
memstream_write(int fd, void* data, size_t len, void* arg) {
  buffer* b = arg;
  memstream* ms = b->cookie; //(memstream*)&b[1];
  size_t n = *ms->psize;

  if(alloc_re(ms->pdata, n, n + len)) {
    byte_copy((*ms->pdata) + n, len, data);

    *ms->psize = n += len;
    return len;
  }

  return 0;
}

static inline buffer*
memstream_open(char** ptr, size_t* sizeloc) {
  buffer* b = alloc(sizeof(buffer) + sizeof(memstream));
  memstream* ms = (memstream*)&b[1];

  ms->pdata = ptr;
  ms->psize = sizeloc;

  buffer_init(b, &memstream_write, -1, alloc(1024), 1024);

  b->cookie = ms;
  b->deinit = &memstream_free;

  return b;
}

ssize_t buffer_dummyread(fd_type fd, void* buf, size_t len, void* arg);

static inline buffer*
memstream_reopen(buffer* f, char** buf, size_t* size) {
  buffer nb;
  buffer_flush(f);

  buffer_copybuf(&nb, *buf, *size);

  /*  if((nb = alloc_zero(sizeof(buffer)))) {
      buffer_init(nb, &buffer_dummyread, -1, *buf, *size);

      nb->x = *buf;
      nb->p = 0;
      nb->n = nb->a = *size;
      nb->fd = -1;
      nb->op = &buffer_dummyreadbuf;
      nb->deinit = &memstream_free;
    }

    buffer_free(f);*/

  *f = nb;

  return f;
}

#endif /* !defined(MEMSTREAM_H) */
