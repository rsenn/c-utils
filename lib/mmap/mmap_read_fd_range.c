#include "../windoze.h"

#include "../io_internal.h"
#include "../mmap.h"
#include "../open.h"
#if WINDOWS_NATIVE
#include <windows.h>
#include <io.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#endif

extern char mmap_empty[];

char*
mmap_read_fd_range(fd_t fd, size_t* size, size_t offset, size_t len) {
  char* map;
#if WINDOWS_NATIVE
  /*HANDLE h = (HANDLE)_get_osfhandle((int)fd);
  HANDLE m;
  m = CreateFileMapping(h, 0, PAGE_READONLY, 0, 0, NULL);
  map = 0;
  if(m) {
    DWORD lo = offset;
    DWORD hi = offset >> sizeof(DWORD) * 8;
    DWORD filesize = GetFileSize((HANDLE)fd, NULL);
    *size = filesize > len ? len : filesize;

    map = MapViewOfFile(m, FILE_MAP_READ, hi, lo, *size);
  }
  CloseHandle(m);*/
#else
  struct stat st;
  map = mmap_empty;

  if(fstat(fd, &st) == 0 && (*size = st.st_size)) {
    if(*size > len)
      *size = len;

    map = (char*)mmap(0, *size, PROT_READ, MAP_SHARED, fd, offset);
    if(map == (char*)-1)
      map = 0;
  }
#endif
  return map;
}
