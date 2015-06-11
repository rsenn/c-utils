#include <sys/types.h>

#ifndef _WIN32
#include <unistd.h>
#endif
#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/types.h>

#include <sys/mman.h>
#endif
#include "open.h"
#include "mmap.h" 
#include "uint64.h"

#if defined(__x86_64__) && defined(__linux)
#define mmap mmap64
#endif

char* mmap_map(int fd, size_t sz, uint64 offset) {
#if defined(_WIN32)
  HANDLE m;
  DWORD szl, szh;
  char* map;
  szl = GetFileSize((void*)(ssize_t)fd, &szh);
  m=CreateFileMapping((HANDLE)(intptr_t)fd, 0, PAGE_WRITECOPY, szh, szl, NULL);
  map = 0;
  if(m) { 
   map=MapViewOfFile(m, FILE_MAP_COPY, (offset >> 32), offset & 0xffffffff, sz);
  }
  CloseHandle(m);
  return map;
#else
  char* map;
  map = (char *)mmap(0, sz, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, offset);
  if(map == (char *) - 1)
    map = 0;
  return map;
#endif
}
