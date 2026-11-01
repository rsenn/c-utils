#define _FILE_OFFSET_BITS 64

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "../cdb.h"
#include "../cdb_make.h"
#ifdef __MINGW32__
#include "../windows.h"
#endif

int cdb_make_start(struct cdb_make *c,int64 fd) {
  c->head = 0;
  c->split = 0;
  c->hash = 0;
  c->numentries = 0;
  c->fd = fd;
  c->pos = sizeof c->final;
  buffer_init(&c->b,(void*)write,fd,c->bspace,sizeof c->bspace);
#ifdef __MINGW32__
  return SetFilePointer((HANDLE)(uintptr_t)fd,c->pos,0,FILE_BEGIN);
#else
  return lseek(fd,c->pos,SEEK_SET);
#endif
}
