#include <sys/types.h>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <sys/mman.h>
#endif
#include "../io_internal.h"

extern void io_dontwantread_really(fd_t d, io_entry* e);
extern void io_dontwantwrite_really(fd_t d, io_entry* e);

void io_close(fd_t d ){
  io_entry* e;
  if((e = iarray_get(io_getfds(), d))) {
    e->inuse = 0;
    e->cookie = 0;
    if(e->kernelwantread) io_dontwantread_really(d, e);
    if(e->kernelwantwrite) io_dontwantwrite_really(d, e);
    if(e->mmapped) {
#if defined(_WIN32) || defined(_WIN64)
      UnmapViewOfFile(e->mmapped);
      CloseHandle(e->mh);
#else
      munmap(e->mmapped, e->maplen);
#endif
      e->mmapped = 0;
    }
  }
  close(d);
}
