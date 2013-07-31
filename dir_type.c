#include "dir_internal.h"

int dir_type(struct dir *d)
{
  int r=0;
#if defined(__MINGW32__) || defined(__MSYS__)
  if((d)->dir_finddata.dwFileAttributes & 0x10)
    r |= D_DIRECTORY;
  else if((d)->dir_finddata.dwFileAttributes & 0x20)
    r |= D_FILE;
#else
  if(S_ISDIR((d)->dir_entry->st_mode))
    r |= D_DIRECTORY;
  if(S_ISREG((d)->dir_entry->st_mode))
    r |= D_FILE;
	
#endif
  return r;
}
