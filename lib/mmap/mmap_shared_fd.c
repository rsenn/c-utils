#include "../windoze.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "../mmap.h"
#include "../open.h"

#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

char*
mmap_shared_fd(fd_t fd, size_t* filesize) {
#if WINDOWS_NATIVE
  HANDLE m;
  char* map;
  if(fd == INVALID_HANDLE_VALUE) return 0;
  m = CreateFileMapping(fd, 0, PAGE_READWRITE, 0, 0, NULL);
  map = 0;
  if(m)
    if((map = MapViewOfFile(m, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0))) *filesize = GetFileSize(fd, NULL);
  CloseHandle(m);
  CloseHandle(fd);
  return map;
#else
  char* map;
  if(fd >= 0) {
    *filesize = io_seek(fd, 0, SEEK_END);
    map = (char*)mmap(0, *filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(map == (char*)-1) map = 0;
    close(fd);
    return map;
  }
  return 0;
#endif
}