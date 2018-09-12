#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "../mmap.h"
#include "../open.h"

#if defined(_WIN32) || defined(_WIN32) || defined(__MSYS__)
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

extern char mmap_empty[];

char*
mmap_rw_fd(fd_t fd, size_t* filesize) {
#if WINDOWS || defined(__MSYS__)
  HANDLE m;
  char* map;
  m = CreateFileMapping((HANDLE)(size_t)fd, 0, PAGE_READWRITE, 0, 0, NULL);
  map = 0;
  if(m) map = MapViewOfFile(m, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
  CloseHandle(m);
  return map;
#else
  struct stat st;
  char* map = mmap_empty;
  if(fstat(fd, &st) == 0 && (*filesize = st.st_size)) {
    map = mmap(0, *filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(map == (char*)-1) map = 0;
  }
  return map;
#endif
}
