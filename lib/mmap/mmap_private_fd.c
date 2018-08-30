#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include "../mmap.h"
#include "../open.h"

#if(defined(_WIN32) || defined(_WIN64) || defined(__MINGW32__) || defined(__MINGW64__)) && !defined(__CYGWIN__) &&     \
    !defined(__MSYS__)
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

char*
mmap_private_fd(fd_t fd, size_t* filesize) {
#if(defined(_WIN32) || defined(_WIN64) || defined(__MINGW32__) || defined(__MINGW64__)) && !defined(__CYGWIN__) &&     \
    !defined(__MSYS__)
  HANDLE m;
  char* map;
  if(fd == INVALID_HANDLE_VALUE) return 0;
  m = CreateFileMapping(fd, 0, PAGE_WRITECOPY, 0, 0, NULL);
  map = 0;
  if(m)
    if((map = MapViewOfFile(m, FILE_MAP_COPY, 0, 0, 0))) *filesize = GetFileSize(fd, NULL);
  CloseHandle(m);
  CloseHandle(fd);
  return map;
#else
  char* map;
  if(fd >= 0) {
    *filesize = io_seek(fd, 0, SEEK_END);
    map = (char*)mmap(0, *filesize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if(map == (char*)-1) map = 0;
    close(fd);
    return map;
  }
  return 0;
#endif
}
