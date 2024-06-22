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
  /*if(b->x)
    alloc_free(b->x);
  b->x = 0;*/
  alloc_free(b);
}

static inline ssize_t
memstream_write(int fd, void* data, size_t len, void* arg) {
  buffer* b = arg;
  memstream* ms = (memstream*)&b[1];
  size_t n = *ms->psize;

  if(alloc_re(ms->pdata, n, n + len)) {
    byte_copy((*ms->pdata) + n, len, data);

    *ms->psize += len;
    return len;
  }

  return 0;
}

static inline buffer*
memstream_open(char** ptr, size_t* sizeloc) {
  buffer* b = alloc(sizeof(buffer) + sizeof(memstream) + 1024);
  memstream* ms = (memstream*)&b[1];

  ms->pdata = ptr;
  ms->psize = sizeloc;

  buffer_init(b, &memstream_write, -1, (char*)&ms[1], 1024);

  b->cookie = ms;
  b->deinit = &memstream_free;

  return b;
}

/*static inline buffer*
memstream_reopen(buffer* b, char** ptr, size_t* sizeloc) {
  memstream* ms = (memstream*)&b[1];

  ms->pdata = ptr;
  ms->psize = sizeloc;

  return b;
}*/

#endif /* !defined(MEMSTREAM_H) */
