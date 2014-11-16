#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#endif
#if defined(WIN32) || defined(__MINGW32__) || defined(__MSYS__)
#include <windows.h>
#else
#include <sys/mman.h>
#endif
#include "open.h"
#include "mmap.h"

char mmap_empty[] = { 0 };

char* mmap_read_fd(int fd, size_t *filesize)
{
#if defined(WIN32) || defined(__MINGW32__) || defined(__MSYS__)
  HANDLE m;
  char* map;
  m=CreateFileMapping((HANDLE)fd,0,PAGE_READONLY,0,0,NULL);
  map=0;
  if (m)
    map=MapViewOfFile(m,FILE_MAP_READ,0,0,0);
  CloseHandle(m);
  return map;
#else
  struct stat st;
  char *map = mmap_empty;
  if(fstat(fd, &st) == 0 && (*filesize = st.st_size))
  {
    map=mmap(0,*filesize,PROT_READ,MAP_SHARED,fd,0);
    if (map==(char*)-1)
      map=0;
  }
  return map;
#endif
}
