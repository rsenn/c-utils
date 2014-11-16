#if !( defined(__MINGW32__) || defined(__MSYS__))
#include <dirent.h>
#endif
#include "dir_internal.h"

int dir_type(struct dir_s *d)
{
  int r=0;
#if defined(__MINGW32__) || defined(__MSYS__)
  if(dir_INTERNAL(d)->dir_finddata.dwFileAttributes & 0x10)
    r |= D_DIRECTORY;
  else if(dir_INTERNAL(d)->dir_finddata.dwFileAttributes & 0x20)
    r |= D_FILE;
#else
#ifndef DT_DIR
#define DT_DIR 4
#endif
#ifndef DT_REG
#define DT_REG 8
#endif
#ifndef DT_LNK
#define DT_LNK 10
#endif
  if((dir_INTERNAL(d)->dir_entry->d_type) == DT_DIR)
    r |= D_DIRECTORY;
  if((dir_INTERNAL(d)->dir_entry->d_type) == DT_REG)
    r |= D_FILE;
  if((dir_INTERNAL(d)->dir_entry->d_type) == DT_LNK)
    r |= D_SYMLINK;
	
#endif
  return r;
}
