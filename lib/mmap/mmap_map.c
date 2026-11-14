#include "../windoze.h"

#include "../io_internal.h"
#if WINDOWS_NATIVE
#include <windows.h>
#include <io.h>
#else
#include <sys/mman.h>
#include <sys/types.h>
#endif

#include "../mmap.h"
#include "../open.h"

char*
mmap_map(fd_type fd, size_t sz, uint64 offset) {
#if WINDOWS_NATIVE
  HANDLE m, h = (HANDLE)_get_osfhandle((int)fd);
  char* map = 0;
  DWORD szh, szl = GetFileSize(h, &szh);
  
  if((m = CreateFileMapping(h, 0, PAGE_WRITECOPY, szh, szl, NULL))) {
    map = MapViewOfFile(m, FILE_MAP_COPY, (offset >> 32), offset & 0xffffffff, sz);

    CloseHandle(m);
  }
#else
  char* map = (char*)mmap(0, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, offset);

  if(map == (char*)-1)
    map = 0;
#endif

  return map;
}
