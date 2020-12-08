#include "../windoze.h"
#include "../buffer.h"
#include "../open.h"
#include "../io_internal.h"

#if WINDOWS_NATIVE
#ifdef _MSC_VER
#define _CRT_INTERNAL_NONSTDC_NAMES 1
#endif
#if !defined(__LCC__) && !defined(__MINGW32__)
#define read _read
#define write _write
#define open _open
#define close _close
#endif
#include <io.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h>
#include "../alloc.h"

int
buffer_write_fd(buffer* b, fd_t fd) {
  b->fd = fd;

  b->p = b->n = 0;
  b->a = BUFFER_OUTSIZE;
  b->x = (char*)alloc(BUFFER_OUTSIZE);

  if(b->x == NULL)
    return -1;
  b->op = (buffer_op_proto*)(void*)&write;
  b->deinit = (void (*)()) & buffer_free;
  return 0;
}
