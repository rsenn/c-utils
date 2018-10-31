#include "../windoze.h"

#include "../io.h"
#if WINDOWS_NATIVE
#include <windows.h>
#include <io.h>
#else
#include <sys/mman.h>
#include <sys/types.h>
#endif

#include "../mmap.h"
#include "../open.h"
//#include "../uint64.h"

char*
mmap_map(fd_t fd, size_t sz, uint64 offset) {
#if WINDOWS_NATIVE
  HANDLE h = (HANDLE)_get_osfhandle(fd);
  HANDLE m;
  DWORD szl, szh;
  char* map;
  szl = GetFileSize(h, &szh);
  m = CreateFileMapping(h, 0, PAGE_WRITECOPY, szh, szl, NULL);
  map = 0;
  if(m) {
    map = MapViewOfFile(m, FILE_MAP_COPY, (offset >> 32), offset & 0xffffffff, sz);

    CloseHandle(m);
  }
#else
  char* map;
  map = (char*)mmap(0, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, offset);
  if(map == (char*)-1) map = 0;
#endif
  return map;
}
