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

ssize_t fd_filesize(fd_type);

char mmap_empty[] = {0};

char*
mmap_read_fd(fd_type fd, size_t* filesize) {
#if WINDOWS_NATIVE
  HANDLE h = (HANDLE)_get_osfhandle((int)fd);
  HANDLE m;
  char* map;
  m = CreateFileMapping(h, 0, PAGE_READONLY, 0, 0, NULL);
  map = 0;

  if(m) {
    if((map = MapViewOfFile(m, FILE_MAP_READ, 0, 0, 0)))
      *filesize = GetFileSize((HANDLE)fd, NULL);
  }
  CloseHandle(m);
  return map;
#else
  struct stat st;
  char* map = mmap_empty;

  if((ssize_t)(*filesize = fd_filesize(fd)) >= 0) {
    map = (char*)mmap(0, *filesize, PROT_READ, MAP_SHARED, fd, 0);

    if(map == (char*)-1)
      map = 0;
  }

  return map;
#endif
}
