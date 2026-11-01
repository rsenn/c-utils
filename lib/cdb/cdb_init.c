#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "../byte.h"
#include "../cdb.h"
#include "../havepread.h"
#ifdef __MINGW32__
#include "../windows.h"
#else
#include <sys/mman.h>
#endif

void cdb_init(struct cdb *c,int64 fd) {
#ifndef __MINGW32__
  struct stat st;
  char *x;
#endif

  cdb_free(c);
  cdb_findstart(c);
  c->fd = fd;

#ifdef __MINGW32__
  {
    HANDLE m=CreateFileMapping((HANDLE)(uintptr_t)fd,0,PAGE_READONLY,0,0,NULL);
    if (m)
      if ((c->map=MapViewOfFile(m,FILE_MAP_READ,0,0,0)))
	c->size=GetFileSize((HANDLE)(uintptr_t)fd,NULL);
    CloseHandle(m);
  }
#else
  if (fstat(fd,&st) == 0)
    if (st.st_size <= 0xffffffff) {
      x = mmap(0,st.st_size,PROT_READ,MAP_SHARED,fd,0);
      if (x != MAP_FAILED) {
	c->size = st.st_size;
	c->map = x;
      }
    }
#endif
}
