#include "../windoze.h"

#include "../io.h"
#include "../mmap.h"
#include "../open.h"
#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

char*
mmap_shared(const char* filename, size_t* filesize) {
#if WINDOWS_NATIVE
  HANDLE fd, m;
  char* map;
  fd = CreateFile(filename,
                  GENERIC_READ | GENERIC_WRITE,
                  FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                  0,
                  OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL,
                  0);
  if(fd == INVALID_HANDLE_VALUE)
    return 0;
  m = CreateFileMapping(fd, 0, PAGE_READWRITE, 0, 0, NULL);
  map = 0;
  if(m)
    if((map = MapViewOfFile(m, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0)))
      *filesize = GetFileSize(fd, NULL);
  CloseHandle(m);
  CloseHandle(fd);
  return map;
#else
  fd_t fd = open_rw(filename);
  char* map;
  if(fd >= 0) {
    register off_t o = lseek(fd, 0, SEEK_END);
    if(sizeof(off_t) != sizeof(size_t) && o > (off_t)(size_t)-1) {
      close(fd);
      return 0;
    }
    *filesize = (size_t)o;
    map = mmap(0, *filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(map == (char*)-1)
      map = 0;
    close(fd);
    return map;
  }
  return 0;
#endif
}
