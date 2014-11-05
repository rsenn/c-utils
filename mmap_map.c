#include <sys/types.h>
#include <unistd.h>
#ifdef __MINGW32__
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/mman.h>
#endif
#include "open.h"
#include "mmap.h"

#if defined(__x86_64__) && defined(__linux)
#define mmap mmap64
#endif

char* mmap_map(int fd, size_t sz, uint64 offset) {
#ifdef __MINGW32__
  HANDLE m;
  DWORD szl,szh;
  char* map;
  szl = GetFileSize(fd,&szh);
  m=CreateFileMapping(fd,0,PAGE_WRITECOPY,szh,szl,NULL);
  map=0;
  if(m){ 
   map=MapViewOfFile(m,FILE_MAP_COPY,offset>>32,offset,sz);
  }
  CloseHandle(m);
  return map;
#else
  char *map;
  map = (char*)mmap(0,sz,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,offset);
  if(map == (char*)-1)
    map = 0;
  return map;
#endif
}
