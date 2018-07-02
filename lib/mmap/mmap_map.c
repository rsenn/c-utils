#include <sys/types.h>

#if !(defined(_WIN32) || defined(_WIN64))
#include <unistd.h>
#endif
#if defined(_WIN32)
#include <windows.h>
#include <stdio.h>
#else
#include <sys/types.h>

#include <sys/mman.h>
#endif
#include "../open.h"
#include "../mmap.h" 
#include "../uint64.h"
/*
#if defined(__x86_64__) && defined(__linux) && !defined(__dietlibc__)
#define mmap mmap64
#endif
*/
char* mmap_map(int fd, size_t sz, uint64 offset) {
#if defined(_WIN32)
  HANDLE h = _get_osfhandle(fd), m;
  DWORD szl, szh;
  char* map;
  szl = GetFileSize(h, &szh);
  m=CreateFileMapping(h, 0, PAGE_WRITECOPY, szh, szl, NULL);
  map = 0;
  if(m) { 
   map=MapViewOfFile(m, FILE_MAP_COPY, (offset >> 32), offset & 0xffffffff, sz);
//   //if(map == NULL)  fprintf(stderr, "MapViewOfFile(%p, %i, 0x%08x, 0x%08x, %lu) = NULL\n",  m, FILE_MAP_COPY, (offset >> 32), offset & 0xffffffff, sz);
//  } //else fprintf(stderr, "CreateFileMapping(%p, %i, %i, 0x%08x, 0x%08x, NULL) = NULL\n",  h, 0, PAGE_WRITECOPY, szh, szl);

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
