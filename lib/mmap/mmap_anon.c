#define _GNU_SOURCE
#include <sys/types.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#include "../open.h"
#endif
#include "../mmap.h"

char*
mmap_anon(size_t size) {
#ifdef _WIN32
  HANDLE fd, m;
  char* map;
  fd = CreateFile(filename,
                  GENERIC_READ,
                  FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                  0,
                  OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL,
                  0);
  if(fd == INVALID_HANDLE_VALUE)
    return 0;
  m = CreateFileMapping(fd, 0, PAGE_WRITECOPY, 0, 0, NULL);
  map = 0;
  if(m)
    if((map = MapViewOfFile(m, FILE_MAP_COPY, 0, 0, 0)))
      *filesize = GetFileSize(fd, NULL);
  CloseHandle(m);
  CloseHandle(fd);
  return map;
#else
  char* map;

  map = (char*)mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
  if(map == (char*)-1)
    map = 0;
  return map;

#endif
}
