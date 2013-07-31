#include <sys/types.h>
#include <unistd.h>
#ifdef __MINGW32__
#include <windows.h>
#else
#include <sys/mman.h>
#endif
#include "open.h"
#include "mmap.h"

char* mmap_private(const char* filename,unsigned long* filesize) {
#ifdef __MINGW32__
  HANDLE fd,m;
  char* map;
  fd=CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
  if (fd==INVALID_HANDLE_VALUE)
    return 0;
  m=CreateFileMapping(fd,0,PAGE_WRITECOPY,0,0,NULL);
  map=0;
  if (m)
    map=MapViewOfFile(m,FILE_MAP_COPY,0,0,0);
  CloseHandle(m);
  CloseHandle(fd);
  return map;
#else
  int fd=open_read(filename);
  char *map = "";
  if (fd>=0) {
    if((*filesize=lseek(fd,0,SEEK_END)))
    {
      map=mmap(0,*filesize,PROT_WRITE|PROT_READ,MAP_PRIVATE,fd,0);
      if (map==(char*)-1)
        map=0;
    }
    close(fd);
    return map;
  }
  return 0;
#endif
}
