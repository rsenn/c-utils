#include <sys/types.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <io.h>
#include <stdio.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#endif
#include "../open.h"
#include "../mmap.h"
//#include "../uint64.h"

#ifdef __MINGW64__
intptr_t __cdecl __declspec(dllimport) _get_osfhandle(int _FileHandle);
#endif

char* mmap_map(int fd, size_t sz, uint64 offset) {
#if defined(_WIN32) ||  defined(_WIN64)
  HANDLE h = (HANDLE)_get_osfhandle(fd);
  HANDLE m;
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
  }
#else
  char* map;
  map = (char *)mmap(0, sz, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, offset);
  if(map == (char *) - 1)
    map = 0;
#endif
  return map;
}
