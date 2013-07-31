#include "dir_internal.h"

int dir_type(struct dir_s *d)
{
  int r=0;
#if defined(__MINGW32__) || defined(__MSYS__)
  if(((struct dir_internal_s *)(d->dir_int))->dir_finddata.dwFileAttributes & 0x10)
    r |= D_DIRECTORY;
  else if(((struct dir_internal_s *)(d->dir_int))->dir_finddata.dwFileAttributes & 0x20)
    r |= D_FILE;
#else
  if(S_ISDIR(((struct dir_internal_s *)(d->dir_int))->dir_entry->st_mode))
    r |= D_DIRECTORY;
  if(S_ISREG(((struct dir_internal_s *)(d->dir_int))->dir_entry->st_mode))
    r |= D_FILE;
	
#endif
  return r;
}
