#include "../io.h"
#include "../mmap.h"
#include "../open.h"
#include "../windoze.h"
#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#endif

extern char mmap_empty[];

char*
mmap_rw_fd(fd_t fd, size_t* filesize) {
#if WINDOWS_NATIVE
  HANDLE m;
  char* map;
  m = CreateFileMapping((HANDLE)(size_t)fd, 0, PAGE_READWRITE, 0, 0, NULL);
  map = 0;
  if(m) map = MapViewOfFile(m, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
  CloseHandle(m);
  return map;
#else
  struct stat st;
  char* map = mmap_empty;
  if(fstat(fd, &st) == 0 && (*filesize = st.st_size)) {
    map = mmap(0, *filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(map == (char*)-1) map = 0;
  }
  return map;
#endif
}
